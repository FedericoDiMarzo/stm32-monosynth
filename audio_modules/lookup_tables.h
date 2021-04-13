

#ifndef STM32_MONOSYNTH_LOOKUP_TABLES_H
#define STM32_MONOSYNTH_LOOKUP_TABLES_H

#include "../audio/audio_math.h"
#include <cmath>

#define LUT_TABLE_RESOLUTION 256
#define AUDIOMATH_LOG20 2.995732273553991f
#define AUDIOMATH_LOG20000 9.903487552536127

/**
 * This header defines various specific lookup tables.
 */
namespace AudioMath {
    /**
     * Sine function.
     */
    extern LookupTable<LUT_TABLE_RESOLUTION> sineLut;

    /**
     * Ramp function, periodic between 0 and 1.
     */
    extern LookupTable<LUT_TABLE_RESOLUTION> rampLut;

    /**
     * Exponential function. The input is bounded between log20 and log20000.
     * Useful to obtain a logarithmic potentiometer.
     */
    extern LookupTable<LUT_TABLE_RESOLUTION> expLut;
}

#endif //STM32_MONOSYNTH_LOOKUP_TABLES_H
