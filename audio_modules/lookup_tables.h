

#ifndef STM32_MONOSYNTH_LOOKUP_TABLES_H
#define STM32_MONOSYNTH_LOOKUP_TABLES_H

#include "../audio/audio_math.h"
#include <cmath>

#define LUT_TABLE_RESOLUTION 256

namespace AudioMath {
    LookupTable<LUT_TABLE_RESOLUTION> sineLut([](float x) { return std::sin(x); },
                                              0, 2 * M_PI,
                                              LookupTableEdges::PERIODIC);
}

#endif //STM32_MONOSYNTH_LOOKUP_TABLES_H
