/***************************************************************************
 *   Copyright (C) 2012 by Terraneo Federico                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <stdexcept>
#include <memory>
#include <cstdio>
#include <cstring>
#include <sys/wait.h>
#include <signal.h>
#include "sync.h"
#include "process_pool.h"
#include "process.h"

using namespace std;

#ifdef WITH_PROCESSES

/*
 * List of implemented supervisor calls
 * ------------------------------------
 * 
 * 0 : Yield. Can be called both by kernel threads and process threads both in
 *     userspace and kernelspace mode. It causes the scheduler to switch to
 *     another thread. It is the only SVC that is available also when processes
 *     are disabled in miosix_config.h. No parameters, no return value.
 * 1 : Back to userspace. It is used by process threads running in kernelspace
 *     mode to return to userspace mode after completing a supervisor call.
 *     If called by a process thread already in userspace mode it does nothing.
 *     Use of this SVC is by kernel threads is forbidden. No parameters, no
 *     return value.
 * 2 : Exit. Terminates the current process. One parameter, the exit code.
 *     Never returns. Use of this SVC is by kernel threads is forbidden.
 * 3 : Write. Writes to stdout or a file. Three parameters, file descriptor,
 *     pointer to data to be written, size of data. Returns the number of
 *     written data or -1 on error. Use of this SVC is by kernel threads is
 *     forbidden.
 * 4 : Read. Reads from stdin or a file. Three parameters, file descriptor,
 *     pointer to data buffer, size of buffer. Returns the number of
 *     read data or -1 on error. Use of this SVC is by kernel threads is
 *     forbidden.
 * 5 : Usleep. One parameter, number of microseconds to sleep. Returns 0 on
 *     success, -1 on failure. Use of this SVC is by kernel threads isforbidden.
 */

namespace miosix {

//
// class Process
//

pid_t Process::create(const ElfProgram& program)
{
    auto_ptr<Process> proc(new Process(program));
    {   
        Lock<Mutex> l(procMutex);
        proc->pid=getNewPid();
        if(Thread::getCurrentThread()->proc!=0)
            proc->ppid=Thread::getCurrentThread()->proc->pid;
        else proc->ppid=0;
        processes[proc->pid]=proc.get();
    }
    Thread *thr=Thread::createUserspace(Process::start,0,Thread::DEFAULT,
        proc.get());
    if(thr==0)
    {
        Lock<Mutex> l(procMutex);
        processes.erase(proc->pid);
        throw runtime_error("Thread creation failed");
    }
    //Cannot throw bad_alloc due to the reserve in Process's constructor.
    //This ensures we will never be in the uncomfortable situation where a
    //thread has already been created but there's no memory to list it
    //among the threads of a process
    proc->threads.push_back(thr);
    thr->wakeup(); //Actually start the thread, now that everything is set up
    pid_t result=proc->pid;
    proc.release(); //Do not delete the pointer
    return result;
}

pid_t Process::getppid(pid_t proc)
{
    Lock<Mutex> l(procMutex);
    map<pid_t,Process *>::iterator it=processes.find(proc);
    if(it==processes.end()) return -1;
    return it->second->ppid;
}

pid_t Process::waitpid(pid_t pid, int* exit, int options)
{
    Lock<Mutex> l(procMutex);
    pid_t result=-1;
    if(pid<=0)
    {
        //Wait for a generic child process
        Process *self=Thread::getCurrentThread()->proc;
        pid_t ppid=self==0 ? 0 : self->pid;
        multimap<pid_t,Process *>::iterator it=zombies.find(ppid);
        if((options & WNOHANG) && it==zombies.end()) return 0;
        while(it==zombies.end())
        {
            genericWaiting.wait(l);
            it=zombies.find(ppid);
        }
        Process *joined=it->second;
        if(joined->waitCount!=0) errorHandler(UNEXPECTED);
        result=joined->pid;
        if(exit!=0) *exit=joined->exitCode;
        zombies.erase(it);
        processes.erase(result);
        delete joined;
    } else {
        //Wait on a specific process
        map<pid_t,Process *>::iterator it=processes.find(pid);
        if(it!=processes.end())
        {
            Process *joined=it->second;
            if(joined->exitCode==-1)
            {
                //Process hasn't terminated yet
                if(options & WNOHANG) return 0;
                joined->waitCount++;
                joined->waiting.wait(l);
                joined->waitCount--;
                if(joined->waitCount<0 || joined->exitCode==-1)
                    errorHandler(UNEXPECTED);
            }
            result=joined->pid;
            if(exit!=0) *exit=joined->exitCode;
            if(joined->waitCount==0)
            {
                typedef typename multimap<pid_t,Process *>::iterator iterator;
                pair<iterator,iterator> p=zombies.equal_range(joined->ppid);
                for(iterator it=p.first;it!=p.second;++it)
                {
                    if(it->second!=joined) continue;
                    zombies.erase(it);
                    break;
                }
                processes.erase(result);
                delete joined;
            }
        }
    }
    return result;
}

Process::~Process()
{
    #ifdef __CODE_IN_XRAM
    ProcessPool::instance().deallocate(loadedProgram);
    #endif //__CODE_IN_XRAM
}

Process::Process(const ElfProgram& program) : program(program), waitCount(0),
        exitCode(-1)
{
    //This is required so that bad_alloc can never be thrown when the first
    //thread of the process will be stored in this vector
    threads.reserve(1);
    //Done here so if not enough memory the new process is not even created
    image.load(program);
    unsigned int elfSize=program.getElfSize();
    unsigned int roundedSize=elfSize;
    //Allocatable blocks must be greater than ProcessPool::blockSize, and must
    //be a power of two due to MPU limitations
    if(elfSize<ProcessPool::blockSize) roundedSize=ProcessPool::blockSize;
    else if(elfSize & (elfSize-1)) roundedSize=1<<ffs(elfSize);
    #ifndef __CODE_IN_XRAM
    //FIXME -- begin
    //Till a flash file system that ensures proper alignment of the programs
    //loaded in flash is implemented, make the whole flash visible as a big MPU
    //region
    extern unsigned char _end asm("_end");
    unsigned int flashEnd=reinterpret_cast<unsigned int>(&_end);
    if(flashEnd & (flashEnd-1)) flashEnd=1<<ffs(flashEnd);
    mpu=miosix_private::MPUConfiguration(0,flashEnd,
            image.getProcessBasePointer(),image.getProcessImageSize());
//    mpu=miosix_private::MPUConfiguration(program.getElfBase(),roundedSize,
//            image.getProcessBasePointer(),image.getProcessImageSize());
    //FIXME -- end
    #else //__CODE_IN_XRAM
    loadedProgram=ProcessPool::instance().allocate(roundedSize);
    memcpy(loadedProgram,reinterpret_cast<char*>(program.getElfBase()),elfSize);
    mpu=miosix_private::MPUConfiguration(loadedProgram,roundedSize,
            image.getProcessBasePointer(),image.getProcessImageSize());
    #endif //__CODE_IN_XRAM
}

void *Process::start(void *argv)
{
    Process *proc=Thread::getCurrentThread()->proc;
    if(proc==0) errorHandler(UNEXPECTED);
    unsigned int entry=proc->program.getEntryPoint();
    #ifdef __CODE_IN_XRAM
    entry=entry-proc->program.getElfBase()+
        reinterpret_cast<unsigned int>(proc->loadedProgram);
    #endif //__CODE_IN_XRAM
    Thread::setupUserspaceContext(entry,proc->image.getProcessBasePointer(),
        proc->image.getProcessImageSize());
    int returnValue=0;
    bool running=true;
    do {
        miosix_private::SyscallParameters sp=Thread::switchToUserspace();
        if(proc->fault.faultHappened())
        {
            running=false;
            returnValue=SIGSEGV; //Segfault
            #ifdef WITH_ERRLOG
            iprintf("Process %d terminated due to a fault\n"
                    "* Code base address was 0x%x\n"
                    "* Data base address was %p\n",proc->pid,
                    #ifndef __CODE_IN_XRAM
                    proc->program.getElfBase(),
                    #else //__CODE_IN_XRAM
                    reinterpret_cast<unsigned int>(proc->loadedProgram),
                    #endif //__CODE_IN_XRAM
                    proc->image.getProcessBasePointer());
            proc->mpu.dumpConfiguration();
            proc->fault.print();
            #endif //WITH_ERRLOG
        } else {
            switch(sp.getSyscallId())
            {
                case 2:
                    running=false;
                    returnValue=(sp.getFirstParameter() & 0xff)<<8;
                    break;
                case 3:
                    //FIXME: check that the pointer belongs to the process
                    sp.setReturnValue(write(sp.getFirstParameter(),
                        reinterpret_cast<const char*>(sp.getSecondParameter()),
                        sp.getThirdParameter()));
                    break;
                case 4:
                    //FIXME: check that the pointer belongs to the process
                    sp.setReturnValue(read(sp.getFirstParameter(),
                        reinterpret_cast<char*>(sp.getSecondParameter()),
                        sp.getThirdParameter()));
                    break;
                case 5: 
                    sp.setReturnValue(usleep(sp.getFirstParameter()));
                    break;
                default:
                    running=false;
                    returnValue=SIGSYS; //Bad syscall
                    #ifdef WITH_ERRLOG
                    iprintf("Unexpected syscall number %d\n",sp.getSyscallId());
                    #endif //WITH_ERRLOG
                    break;
            }
        }
        if(Thread::testTerminate()) running=false;
    } while(running);
    {
        Lock<Mutex> l(procMutex);
        proc->exitCode=returnValue;
        if(proc->waitCount>0) proc->waiting.broadcast();
        else {
            zombies.insert(make_pair(proc->ppid,proc));
            genericWaiting.broadcast();
        }
    }
    return 0;
}

pid_t Process::getNewPid()
{
    for(;;pidCounter++)
    {
        if(pidCounter<0) pidCounter=1;
        if(pidCounter==0) continue; //Zero is not a valid pid
        map<pid_t,Process*>::iterator it=processes.find(pidCounter);
        if(it!=processes.end()) continue; //Pid number already used
        return pidCounter++;
    }
}

map<pid_t,Process*> Process::processes;
multimap<pid_t,Process *> Process::zombies;
pid_t Process::pidCounter=1;
Mutex Process::procMutex;
ConditionVariable Process::genericWaiting;
    
} //namespace miosix

#endif //WITH_PROCESSES