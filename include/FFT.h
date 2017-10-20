#ifndef __FFT_H__
#define __FFT_H__

#include <iostream>
#include <stdint.h>
#include <vector>
#include <complex>
#include <cmath>

typedef std::complex<double> complexdbl;

void bitReversalSwap(std::vector<complexdbl> data);
std::vector<complexdbl> getTwiddleFactors(uint16_t N);

#endif
