#define mic_distance 0.035
#define Vs 340//speed of sound
#define range 100
#define average_filter_length 40
void visualize(double theta);
void wave_to_chs(int start,int show_data);
void data_print();
void record();
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

    int16_t block[16000 * 8];
} Data_t;

//typedef struct WAV_data_block {
//} Data_block_t;

typedef struct WAV_fotmat
{
    RIFF_t riff;
    FMT_t fmt;
    Data_t data;
} Wav;

typedef struct Wave_ch1234_format
{
    int16_t ch1[range] = {0};
    int16_t ch2[range] = {0};
    int16_t ch3[range] = {0};
    int16_t ch4[range] = {0};

} Wave1234;

typedef struct Delayxx_format
{
    int delay;
    double cc;
} delayxx;



typedef struct Delay_ch1234_format
{
    delayxx delay12;
    delayxx delay13;
    delayxx delay14;
    delayxx delay23;
    delayxx delay24;
    delayxx delay34;
} Delay1234;
Delay1234 delay1234;