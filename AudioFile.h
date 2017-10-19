#ifndef __AUDIO_FILE_H__
#define __AUDIO_FILE_H__

#include <iostream>
#include <string>
#include <stdint.h>

enum
{
    DATA_TYPE_8,
    DATA_TYPE_16,
    DATA_TYPE_32
};

enum
{
    ELOAD_NONE,
    ELOAD_WRONG_CHUNKDESC,
    ELOAD_WRONG_FORMAT,
    ELOAD_WRONG_FORMATHEADER,
    ELOAD_UNSUPPORTED_DATAHEADER
};

class WavFile
{
public:
    WavFile(char* fpath);
    ~WavFile();

    bool         is_ok() { return this->is_loaded; }
    uint16_t     getLoadedErrorCode() { return (uint16_t) this->load_error_code; }
    std::string  chunkDescription() { return this->chunkdesc; }
    uint32_t     chunkSize() { return this->chunksize; }
    std::string  format() { return this->fmt; }
    std::string  formatHeader() { return this->fmt_hdr; }
    uint32_t     formatSubchunkSize() { return this->fmt_subchunksize; }
    uint16_t     formatId() { return this->audiofmt; }
    uint16_t     numChannels() { return this->num_ch; }
    uint32_t     sampleRate() { return this->sample_rate; }
    uint32_t     byteRate() { return this->byte_rate; }
    uint16_t     blockAlign() { return this->block_align; }
    uint16_t     bitPerSample() { return this->bit_per_sample; }
    std::string  dataHeader() { return this->data_hdr; }
    uint32_t     dataSubchunkSize() { return this->data_subchunksize; }

private:
    std::string  chunkdesc;
    uint32_t     chunksize;
    std::string  fmt;
    std::string  fmt_hdr;
    uint32_t     fmt_subchunksize;
    uint16_t     audiofmt;
    uint16_t     num_ch;
    uint32_t     sample_rate;
    uint32_t     byte_rate;
    uint16_t     block_align;
    uint16_t     bit_per_sample;
    std::string  data_hdr;
    uint32_t     data_subchunksize;

    bool         is_loaded;
    uint8_t      load_error_code;
};

#endif
