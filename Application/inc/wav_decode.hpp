#ifndef _WAV_DECODE_
#define _WAV_DECODE_

#include "stdint.h"
#include "stdio.h"
#include <vector>
#include <cmath>
#include <string>
#include "main.hpp"
#include "AlsaCaptureAudio.h"







typedef struct WAV_RIFF
{
    /* chunk "riff" */
    char ChunkID[4]; /* "RIFF" */
    /* sub-chunk-size */
    uint32_t ChunkSize; /* 36 + Subchunk2Size */
    /* sub-chunk-data */
    char Format[4]; /* "WAVE" */
} RIFF_t;

typedef struct WAV_FMT
{
    /* sub-chunk "fmt" */
    char Subchunk1ID[4]; /* "fmt " */
    /* sub-chunk-size */
    uint32_t Subchunk1Size; /* 16 for PCM */
    /* sub-chunk-data */
    uint16_t AudioFormat;   /* PCM = 1*/
    uint16_t NumChannels;   /* Mono = 1, Stereo = 2, etc. */
    uint32_t SampleRate;    /* 8000, 44100, etc. */
    uint32_t ByteRate;      /* = SampleRate * NumChannels * BitsPerSample/8 */
    uint16_t BlockAlign;    /* = NumChannels * BitsPerSample/8 */
    uint16_t BitsPerSample; /* 8bits, 16bits, etc. */
} FMT_t;

typedef struct WAV_data
{
    /* sub-chunk "data" */
    char Subchunk2ID[4]; /* "data" */
    /* sub-chunk-size */
    uint32_t Subchunk2Size; /* data size */
    /* sub-chunk-data */

    int16_t block[SAMPLE_RATE*8*10];
} Data_t;

typedef struct WAV_format
{
    RIFF_t riff;
    FMT_t fmt;
    Data_t data;
} Wav;

typedef struct Wave_ch1234_format
{
    std::vector<float> ch1;
    std::vector<float> ch2;
    std::vector<float> ch3;
    std::vector<float> ch4;

} Wave1234;



class wav_decode :public AlsaCaptureAudio
{
private:
    Wav wav;
    Wave1234 wave1234p;
    int wav_start_point;
    std::string wav_path;

public:
    wav_decode();

    ~wav_decode()
    {
    }

    void record();
    void set_start_point(int set_value);
    void set_wav_path(char* path);
    void read_wav_file();
    Wave1234 * hamming();
    Wave1234 *wave_to_chs(bool show_decoded_data);
};

#endif