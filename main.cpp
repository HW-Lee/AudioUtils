#include <iostream>
#include <fstream>
#include "AudioFile.h"
#include "FFT.h"
#include "CurveFitter.h"

double detectTone(std::vector<double> signal, uint32_t sr);

int main(int argc, char* argv[])
{
    if (argc < 2)
        return 1;

    WavFile wavfile(argv[1]);

    if (wavfile.is_ok()) {
        std::cout << "chunkdesc: " << "\"" << wavfile.chunkDescription() << "\"" << std::endl;
        std::cout << "chunksize: " << wavfile.chunkSize() << std::endl;
        std::cout << "format: " << "\"" << wavfile.format() << "\"" << std::endl;
        std::cout << "fmt_header: " << "\"" << wavfile.formatHeader() << "\"" << std::endl;
        std::cout << "fmt_subchunksize: " << wavfile.formatSubchunkSize() << std::endl;
        std::cout << "audiofmt: " << wavfile.formatId() << std::endl;
        std::cout << "num_ch: " << wavfile.numChannels() << std::endl;
        std::cout << "sample_rate: " << wavfile.sampleRate() << std::endl;
        std::cout << "byte_rate: " << wavfile.byteRate() << std::endl;
        std::cout << "block_align: " << wavfile.blockAlign() << std::endl;
        std::cout << "bit_per_sample: " << wavfile.bitPerSample() << std::endl;
        std::cout << "data_header: " << "\"" << wavfile.dataHeader() << "\"" << std::endl;
        std::cout << "data_subchunksize: " << wavfile.dataSubchunkSize() << std::endl;
        std::cout << "duration: " << (float) wavfile.dataSubchunkSize() / wavfile.byteRate() << std::endl;
        std::cout << "data_type: " << wavfile.dataType() << std::endl;

        if (wavfile.bitPerSample() == 16) {
            short** data = (short**) wavfile.data();
            uint32_t N = 1024;
            std::vector<double> signal(N);

            std::ofstream f("result.log");
            for (uint32_t offset = 0; offset < wavfile.dataSize(); offset+=N) {
                for (uint32_t i = 0; i < N; i++) {
                    if (offset+i >= wavfile.dataSize()) {
                        signal[i] = 0;
                        continue;
                    }
                    signal[i] = data[0][offset+i] / 32768.0;
                }

                double tone_freq = detectTone(signal, wavfile.sampleRate());
                f << tone_freq << std::endl;
            }
            f.close();
        }
    } else {
        std::cout << "WavFile failed to load, error " << wavfile.getLoadedErrorCode() << "(" << wavfile.getErrorMsg() << ")" << std::endl;
    }

    return 0;
}

double detectTone(std::vector<double> signal, uint32_t sr)
{
    uint32_t N = signal.size();

    for (uint32_t i = 0; i < N; i++) {
        signal[i] *= (1 - (double) ((i > N/2) ? (i - N/2):(N/2 - i)) / (N/2));
    }

    std::vector<complexdbl> spectrum = FFT::transform<double>(signal, N*8);
    uint32_t max_idx = 0;
    double spectrum_var = 0;
    double spectrum_avg = 0;
    for (uint32_t i = 0; i < spectrum.size()/2; i++) {
        spectrum_avg += std::abs(spectrum[i]);
        spectrum_var += std::abs(spectrum[i])*std::abs(spectrum[i]);
        if (std::abs(spectrum[i]) > std::abs(spectrum[max_idx])) {
            max_idx = i;
        }
    }
    spectrum_avg /= (spectrum.size()/2);
    spectrum_var /= (spectrum.size()/2);
    spectrum_var -= spectrum_avg*spectrum_avg;

    if (std::sqrt(spectrum_var)/spectrum_avg < 5.0) {
        return -1;
    }

    if (max_idx > 0 && max_idx < spectrum.size()/2) {
        Point2D<double> p1(max_idx-1, std::abs(spectrum[max_idx-1]));
        Point2D<double> p2(max_idx  , std::abs(spectrum[max_idx  ]));
        Point2D<double> p3(max_idx+1, std::abs(spectrum[max_idx+1]));
        std::vector<double> coeffs = QuadraticFitter::fit(p1, p2, p3);
        Point2D<double> peak = QuadraticFitter::peak(coeffs);

        return peak.x * sr/spectrum.size();
    }

    return (double) max_idx * sr/spectrum.size();
}
