

#ifndef STM32_MONOSYNTH_LOOKUP_TABLES_H
#define STM32_MONOSYNTH_LOOKUP_TABLES_H

#include "../audio/audio_math.h"
#include <cmath>

#define LUT_TABLE_RESOLUTION 256

/**
 * This header defines various specific lookup tables.
 */
namespace AudioMath {
    /**
     * Sine function.
     */
    LookupTable<LUT_TABLE_RESOLUTION> sineLut([](float x) { return std::sin(x); },
                                              0.0f, 2.0f * M_PI,
                                              LookupTableEdges::PERIODIC);

    /**
     * Ramp function, periodic between 0 and 1.
     */
    LookupTable<LUT_TABLE_RESOLUTION> rampLut([](float x) {return x;},
                                              0.0f, 1.0f,
                                              LookupTableEdges::PERIODIC);
}

#endif //STM32_MONOSYNTH_LOOKUP_TABLES_H
