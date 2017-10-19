#ifndef __AUDIO_FILE_CPP__
#define __AUDIO_FILE_CPP__

#include "AudioFile.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

void initSamples(samples* s, uint16_t num_ch, uint32_t data_len)
{
    s->raw_data = (void*) malloc(num_ch * data_len);
    s->data = (void**) malloc(num_ch * sizeof(void*));
    for (int i = 0; i < num_ch; i++)
        s->data[i] = (void*) ((uint8_t*) s->raw_data + i*data_len);
}

WavFile::WavFile(char* fpath)
{
    char s[5] = {0};

    this->is_loaded = false;
    this->load_error_code = ELOAD_NONE;
    this->wavdata.raw_data = NULL;
    this->wavdata.data = NULL;
    this->wavdata.data_type = DATA_TYPE_UNSUPPORTED;
    this->wavdata.data_size = 0;

    std::ifstream audiofile(fpath, std::ios::binary);
    if (!audiofile.good()) {
        this->load_error_code = ELOAD_FILE_NOT_FOUND;
        goto end;
    }
    
    audiofile.read(s, 4);
    this->chunkdesc = std::string(s);
    if (this->chunkdesc != "RIFF") {
        this->load_error_code = ELOAD_WRONG_CHUNKDESC;
        goto end;
    }

    audiofile.read((char*) &this->chunksize, 4);
    
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

    audiofile.read((char*) &this->fmt_subchunksize, 4);
    audiofile.read((char*) &this->audiofmt,         2);
    audiofile.read((char*) &this->num_ch,           2);
    audiofile.read((char*) &this->sample_rate,      4);
    audiofile.read((char*) &this->byte_rate,        4);
    audiofile.read((char*) &this->block_align,      2);
    audiofile.read((char*) &this->bit_per_sample,   2);

    audiofile.read(s, 4);
    this->data_hdr = std::string(s);
    if (this->data_hdr != "data") {
        this->load_error_code = ELOAD_UNSUPPORTED_DATAHEADER;
        goto end;
    }

    audiofile.read((char*) &this->data_subchunksize, 4);
    initSamples(&this->wavdata, this->num_ch, this->data_subchunksize/this->num_ch);

    if (this->bit_per_sample == 8) {
        this->wavdata.data_type = DATA_TYPE_8;
        this->wavdata.data_size = this->data_subchunksize/this->num_ch;
        uint8_t** data = (uint8_t**) this->wavdata.data;
        for (uint32_t i = 0; i < this->wavdata.data_size; i++) {
            for (uint32_t j = 0; j < this->num_ch; j++) {
                audiofile.read((char*) &data[j][i], 1);
            }
        }
    } else if (this->bit_per_sample == 16) {
        this->wavdata.data_type = DATA_TYPE_16;
        this->wavdata.data_size = this->data_subchunksize/this->num_ch/2;
        uint16_t** data = (uint16_t**) this->wavdata.data;
        for (uint32_t i = 0; i < this->wavdata.data_size; i++) {
            for (uint32_t j = 0; j < this->num_ch; j++) {
                audiofile.read((char*) &data[j][i], 2);
            }
        }
    } else if (this->bit_per_sample == 32) {
        this->wavdata.data_type = DATA_TYPE_32;
        this->wavdata.data_size = this->data_subchunksize/this->num_ch/4;
        uint32_t** data = (uint32_t**) this->wavdata.data;
        for (uint32_t i = 0; i < this->wavdata.data_size; i++) {
            for (uint32_t j = 0; j < this->num_ch; j++) {
                audiofile.read((char*) &data[j][i], 4);
            }
        }
    } else {
        this->load_error_code = ELOAD_UNSUPPORTED_DATATYPE;
        goto end;
    }

    this->is_loaded = true;

end:
    audiofile.close();
}

WavFile::~WavFile()
{
    if (!this->wavdata.raw_data)
        free(this->wavdata.raw_data);
    if (!this->wavdata.data)
        free(this->wavdata.data);
}

#endif