/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011, 2012, 2013, 2014                *
 *   by Terraneo Federico                                                  *
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

#ifndef SERIAL_H
#define SERIAL_H

#include "filesystem/console/console_device.h"
#include "LPC213x.h"

namespace miosix {

/**
 * Serial port class for LPC2000 microcontrollers
 */
class LPC2000Serial : public Device
{
public:
    /**
     * Constructor, initializes the serial port.
     * Calls errorHandler(UNEXPECTED) if id is not in the correct range, or when
     * attempting to construct multiple objects with the same id. That is,
     * it is possible to instantiate only one instance of this class for each
     * hardware USART.
     * \param id 0=USART0, 1=USART1
     * \param baudrate serial port baudrate.
     */
    LPC2000Serial(/*int id,*/ int baudrate);
    
    /**
     * Read a block of data
     * \param buffer buffer where read data will be stored
     * \param size buffer size
     * \param where where to read from
     * \return number of bytes read or a negative number on failure
     */
    ssize_t readBlock(void *buffer, size_t size, off_t where);
    
    /**
     * Write a block of data
     * \param buffer buffer where take data to write
     * \param size buffer size
     * \param where where to write to
     * \return number of bytes written or a negative number on failure
     */
    ssize_t writeBlock(const void *buffer, size_t size, off_t where);
    
    /**
     * Write a string.
     * An extension to the Device interface that adds a new member function,
     * which is used by the kernel on console devices to write debug information
     * before the kernel is started or in case of serious errors, right before
     * rebooting.
     * Can ONLY be called when the kernel is not yet started, paused or within
     * an interrupt. This default implementation ignores writes.
     * \param str the string to write. The string must be NUL terminated.
     */
    void IRQwrite(const char *str);
    
    /**
     * Performs device-specific operations
     * \param cmd specifies the operation to perform
     * \param arg optional argument that some operation require
     * \return the exact return value depends on CMD, -1 is returned on error
     */
    int ioctl(int cmd, void *arg);
    
    /**
     * \internal the serial port interrupts call this member function.
     * Never call this from user code.
     */
    void IRQhandleInterrupt();
    
    /**
     * Destructor
     */
    ~LPC2000Serial();
    
private:
    bool serialTxFifoEmpty()
    {
        return U0LSR & (1<<6);
    }
    
    //Configure the software queue here
    static const int SOFTWARE_TX_QUEUE=32;///< Size of tx software queue
    static const int SOFTWARE_RX_QUEUE=32;///< Size of rx software queue

    //The hardware queues cannot be modified, since their length is hardware-specific
    static const int HARDWARE_TX_QUEUE_LENGTH=16;
    static const int HARDWARE_RX_QUEUE_LENGTH=8;

    Mutex txMutex;///< Mutex used to guard the tx queue
    Mutex rxMutex;///< Mutex used to guard the rx queue

    Queue<char,SOFTWARE_TX_QUEUE> txQueue;///< Tx software queue
    Queue<char,SOFTWARE_RX_QUEUE> rxQueue;///< Rx software queue
};

} //namespace miosix

#endif //SERIAL_H