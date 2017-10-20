#ifndef __FFT_CPP__
#define __FFT_CPP__

#include "FFT.h"

std::vector<complexdbl> getTwiddleFactors(uint16_t N)
{
    std::vector<complexdbl> twiddle_factors(N);
    for (uint16_t i = 0; i < N; i++) {
        double radius = 2*M_PI * i/N;
        twiddle_factors[i] = complexdbl(cos(radius), sin(radius));
    }

    return twiddle_factors;
}

#endif
