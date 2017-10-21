#include <iostream>
#include <fstream>
#include "AudioFile.h"
#include "FFT.h"

int main(int argc, char* argv[])
{
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
    } else {
        std::cout << "WavFile failed to load, error " << wavfile.getLoadedErrorCode() << "(" << wavfile.getErrorMsg() << ")" << std::endl;
    }

    return 0;
}
