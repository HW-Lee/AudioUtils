#ifndef __AUDIO_FILE_CPP__
#define __AUDIO_FILE_CPP__

#include "AudioFile.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

template<class T>
T readBytesLittleEndian(std::ifstream &ifs, int N)
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

void initSamples(samples* s, uint16_t num_ch, uint32_t data_len)
{
    s->raw_data = (void*) malloc(num_ch * data_len);
    s->data = (void**) malloc(num_ch * sizeof(void*));
    for (int i = 0; i < num_ch; i++)
        s->data[i] = (void*) ((uint8_t*) s->raw_data + i*data_len);
}

WavFile::WavFile(char* fpath)
{
    this->is_loaded = false;
    this->load_error_code = ELOAD_NONE;
    this->wavdata.data_type = DATA_TYPE_UNSUPPORTED;

    std::ifstream audiofile(fpath, std::ios::binary);
    char s[5] = {0};
    
    audiofile.read(s, 4);
    this->chunkdesc = std::string(s);
    if (this->chunkdesc != "RIFF") {
        this->load_error_code = ELOAD_WRONG_CHUNKDESC;
        goto end;
    }
    
    this->chunksize = readBytesLittleEndian<uint32_t>(audiofile, 4);
    
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

    this->fmt_subchunksize = readBytesLittleEndian<uint32_t>(audiofile, 4);
    this->audiofmt = readBytesLittleEndian<uint16_t>(audiofile, 2);
    this->num_ch = readBytesLittleEndian<uint16_t>(audiofile, 2);
    this->sample_rate = readBytesLittleEndian<uint32_t>(audiofile, 4);
    this->byte_rate = readBytesLittleEndian<uint32_t>(audiofile, 4);
    this->block_align = readBytesLittleEndian<uint16_t>(audiofile, 2);
    this->bit_per_sample = readBytesLittleEndian<uint16_t>(audiofile, 2);

    audiofile.read(s, 4);
    this->data_hdr = std::string(s);
    if (this->data_hdr != "data") {
        this->load_error_code = ELOAD_UNSUPPORTED_DATAHEADER;
        goto end;
    }

    this->data_subchunksize = readBytesLittleEndian<uint32_t>(audiofile, 4);
    initSamples(&this->wavdata, this->num_ch, this->data_subchunksize/this->num_ch);

    if (this->bit_per_sample == 8) {
        this->wavdata.data_type = DATA_TYPE_8;
        this->wavdata.data_size = this->data_subchunksize/this->num_ch;
        uint8_t** data = (uint8_t**) this->wavdata.data;
        for (int i = 0; i < this->wavdata.data_size; i++) {
            for (int j = 0; j < this->num_ch; j++) {
                data[j][i] = readBytesLittleEndian<uint8_t>(audiofile, 1);
            }
        }
    } else if (this->bit_per_sample == 16) {
        this->wavdata.data_type = DATA_TYPE_16;
        this->wavdata.data_size = this->data_subchunksize/this->num_ch/2;
        uint16_t** data = (uint16_t**) this->wavdata.data;
        for (int i = 0; i < this->wavdata.data_size; i++) {
            for (int j = 0; j < this->num_ch; j++) {
                data[j][i] = readBytesLittleEndian<uint16_t>(audiofile, 2);
            }
        }
    } else if (this->bit_per_sample == 32) {
        this->wavdata.data_type = DATA_TYPE_32;
        this->wavdata.data_size = this->data_subchunksize/this->num_ch/4;
        uint32_t** data = (uint32_t**) this->wavdata.data;
        for (int i = 0; i < this->wavdata.data_size; i++) {
            for (int j = 0; j < this->num_ch; j++) {
                data[j][i] = readBytesLittleEndian<uint32_t>(audiofile, 4);
            }
        }
    }

    this->is_loaded = true;

end:
    audiofile.close();
}

WavFile::~WavFile()
{
    if (this->is_loaded) {
        free(this->wavdata.raw_data);
        free(this->wavdata.data);
    }
}

#endif