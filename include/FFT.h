#ifndef __FFT_H__
#define __FFT_H__

#include <iostream>
#include <stdint.h>
#include <vector>
#include <complex>
#include <cmath>

typedef std::complex<double> complexdbl;

class FFT
{
public:
	template<class T>
	static std::vector< std::complex<T> > transform(std::vector< std::complex<T> > signal);
	template<class T>
	static std::vector< std::complex<T> > transform(std::vector<T> signal);

	template<class T>
	static std::vector< std::complex<T> > inv_transform(std::vector< std::complex<T> > signal);
	template<class T>
	static std::vector< std::complex<T> > inv_transform(std::vector<T> signal);
};

uint32_t ceilpw2(uint32_t k);
void bitReversalSwap(std::vector<complexdbl>& data);
std::vector<complexdbl> getTwiddleFactors(uint32_t N);
std::vector<complexdbl> internal_FFT(std::vector<complexdbl> data, std::vector<complexdbl> twiddle);

template<class T>
std::vector<complexdbl> toDouble(std::vector< std::complex<T> > arr)
{
	std::vector<complexdbl> v(arr.size());
	for (uint32_t i = 0; i < arr.size(); i++)
		v[i] = complexdbl((double) arr[i].real(), (double) arr[i].imag());

	return v;
}

template<class T>
std::vector< std::complex<T> > FFT::transform(std::vector< std::complex<T> > signal)
{
	uint32_t N = ceilpw2(signal.size());
	while (signal.size() < N) {
		signal.push_back(std::complex<T>(0));
	}

	std::vector<complexdbl> dbl_signal = toDouble(signal);
	bitReversalSwap(dbl_signal);
	std::vector<complexdbl> twiddle_factors = getTwiddleFactors(N);

	std::vector<complexdbl> dbl_spectrum = internal_FFT(dbl_signal, twiddle_factors);
	std::vector< std::complex<T> > spectrum(N);
	for (uint32_t i = 0; i < N; i++) {
		spectrum[i] = std::complex<T>(dbl_spectrum[i]);
	}

	return spectrum;
}

template<class T>
std::vector< std::complex<T> > FFT::transform(std::vector<T> signal)
{
	std::vector<std::complex<T> > cplx_signal(signal.size());
	for (uint32_t i = 0; i < signal.size(); i++) {
		cplx_signal[i] = std::complex<T>(signal[i]);
	}

	return FFT::transform(cplx_signal);
}

template<class T>
std::vector< std::complex<T> > FFT::inv_transform(std::vector< std::complex<T> > signal)
{
	uint32_t N = ceilpw2(signal.size());
	while (signal.size() < N) {
		signal.push_back(std::complex<T>(0));
	}

	std::vector<complexdbl> dbl_signal = toDouble(signal);
	return signal;
}

template<class T>
std::vector< std::complex<T> > FFT::inv_transform(std::vector<T> signal)
{
	std::vector<std::complex<T> > cplx_signal(signal.size());
	for (uint32_t i = 0; i < signal.size(); i++) {
		cplx_signal[i] = std::complex<T>(signal[i]);
	}

	return FFT::inv_transform(cplx_signal);
}

#endif
