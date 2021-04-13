
#include "lookup_tables.h"

AudioMath::LookupTable<LUT_TABLE_RESOLUTION> AudioMath::sineLut([](float x) { return std::sin(x); },
                                          0.0f, 2.0f * M_PI,
                                          LookupTableEdges::PERIODIC);

AudioMath::LookupTable<LUT_TABLE_RESOLUTION> AudioMath::rampLut([](float x) { return x; },
                                          0.0f, 1.0f,
                                          LookupTableEdges::PERIODIC);


AudioMath::LookupTable<LUT_TABLE_RESOLUTION> AudioMath::expLut([](float x) { return std::exp(x); },
                                         AUDIOMATH_LOG20, AUDIOMATH_LOG20000,
                                         LookupTableEdges::EXTENDED);
