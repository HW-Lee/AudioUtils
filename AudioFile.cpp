#ifndef __AUDIO_FILE_CPP__
#define __AUDIO_FILE_CPP__

#include "AudioFile.h"
#include <iostream>
#include <fstream>

template<class T, unsigned int N>
T readBytesLittleEndian(std::ifstream &ifs)
{
    T v = 0;
    for (int i = 0; i < sizeof(T); i++) {
        if (i < N) {
            uint8_t byte;
            ifs.read((char*) &byte, 1);
            v |= (byte << (sizeof(T)-1)*8);
        }
        if (i < sizeof(T)-1)
            v >>= 8;
    }

    return v;
}

WavFile::WavFile(char* fpath)
{
    this->is_loaded = false;
    this->load_error_code = ELOAD_NONE;

    std::ifstream audiofile(fpath, std::ios::binary);
    char s[5] = {0};
    
    audiofile.read(s, 4);
    this->chunkdesc = std::string(s);
    if (this->chunkdesc != "RIFF") {
        this->load_error_code = ELOAD_WRONG_CHUNKDESC;
        goto end;
    }
    
    this->chunksize = readBytesLittleEndian<uint32_t, 4>(audiofile);
    
    audiofile.read(s, 4);
    this->fmt = std::string(s);
    if (this->fmt != "WAVE") {
        this->load_error_code = ELOAD_WRONG_FORMAT;
        goto end;
    }
    
    audiofile.read(s, 4);
    this->fmt_hdr = std::string(s);
    if (this->fmt_hdr != "fmt ") {
        this->load_error_code = ELOAD_WRONG_FORMATHEADER;
        goto end;
    }

    this->fmt_subchunksize = readBytesLittleEndian<uint32_t, 4>(audiofile);
    this->audiofmt = readBytesLittleEndian<uint16_t, 2>(audiofile);
    this->num_ch = readBytesLittleEndian<uint16_t, 2>(audiofile);
    this->sample_rate = readBytesLittleEndian<uint32_t, 4>(audiofile);
    this->byte_rate = readBytesLittleEndian<uint32_t, 4>(audiofile);
    this->block_align = readBytesLittleEndian<uint16_t, 2>(audiofile);
    this->bit_per_sample = readBytesLittleEndian<uint16_t, 2>(audiofile);

    audiofile.read(s, 4);
    this->data_hdr = std::string(s);
    if (this->data_hdr != "data") {
        this->load_error_code = ELOAD_UNSUPPORTED_DATAHEADER;
        goto end;
    }

    this->data_subchunksize = readBytesLittleEndian<uint32_t, 4>(audiofile);

    this->is_loaded = true;

end:
    audiofile.close();
}

WavFile::~WavFile()
{
}

#endif