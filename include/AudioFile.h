#ifndef __AUDIO_FILE_H__
#define __AUDIO_FILE_H__

#include <string>
#include <stdint.h>

enum
{
    DATA_TYPE_UNSUPPORTED,
    DATA_TYPE_8,
    DATA_TYPE_16,
    DATA_TYPE_32
};

enum
{
    ELOAD_NONE,
    ELOAD_FILE_NOT_FOUND,
    ELOAD_WRONG_CHUNKDESC,
    ELOAD_WRONG_FORMAT,
    ELOAD_WRONG_FORMATHEADER,
    ELOAD_UNSUPPORTED_DATAHEADER,
    ELOAD_UNSUPPORTED_DATATYPE
};

static const char* ERROR_MSGS[] = {
    [ELOAD_NONE]                   = "no error",
    [ELOAD_FILE_NOT_FOUND]         = "input file not found",
    [ELOAD_WRONG_CHUNKDESC]        = "the chunk description is not \"RIFF\"",
    [ELOAD_WRONG_FORMAT]           = "the format is not \"WAVE\"",
    [ELOAD_WRONG_FORMATHEADER]     = "the format chunk does not start with \"fmt \"",
    [ELOAD_UNSUPPORTED_DATAHEADER] = "the data chunk does not start with \"data\"",
    [ELOAD_UNSUPPORTED_DATATYPE]   = "the data resolution is not 8/16/32-bit"
};

typedef struct
{
    void* raw_data;
    void** data;
    uint16_t data_type;
    uint32_t data_size;
} samples;

class WavFile
{
public:
    WavFile(char* fpath);
    ~WavFile();

    bool         is_ok() { return this->is_loaded; }
    std::string  getErrorMsg() { return std::string(ERROR_MSGS[this->load_error_code]); }

    uint16_t     getLoadedErrorCode() { return this->load_error_code; }
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
    void**       data() { return this->wavdata.data; }
    uint16_t     dataType() { return this->wavdata.data_type; }
    uint32_t     dataSize() { return this->wavdata.data_size; }

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
    samples      wavdata;

    bool         is_loaded;
    uint16_t     load_error_code;
};

#endif
