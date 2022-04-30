#include "wav_decode.hpp"
#include "iostream"

using namespace std;

wav_decode::wav_decode()
{
    wave1234p.ch1.resize(RANGE);
    wave1234p.ch2.resize(RANGE);
    wave1234p.ch3.resize(RANGE);
    wave1234p.ch4.resize(RANGE);

    wav_path = "sound_data/test.wav";
    wav_start_point = 0;
}
void wav_decode::record()
{
    // string cmd = "arecord -D hw:0,0 -c 8 -r 44100 -s " + to_string(RANGE) + "  -f  S16_LE  --period-size=1024  --buffer-size=4096 /CHEVY_FYP/sound_data/test.wav &>a.log";
    // system(cmd.data());
    int16_t *buffer = captureAudio();
    for (int i = 0; i < 4 * RANGE; i++)
    {
        if (i % 4 == 0)
        {
            wave1234p.ch1[i / 4] = *(buffer + i);
        }
        if (i % 4 == 1)
        {
            wave1234p.ch2[i / 4] = *(buffer + i);
        }
        if (i % 4 == 2)
        {
            wave1234p.ch3[i / 4] = *(buffer + i);
        }
        if (i % 4 == 3)
        {
            wave1234p.ch4[i / 4] = *(buffer + i);
        }
    }
}
void wav_decode::read_wav_file()
{
    FILE *fp;
    fp = fopen(wav_path.data(), "rb");
    if (!fp)
    {
        cout << "can't open audio file\n";
    }
    fread(&wav, 1, sizeof(wav), fp);
    fclose(fp);
}
Wave1234 *wav_decode::wave_to_chs_4c(bool show_decoded_data)
{
    for (int i = wav_start_point * 4; i < RANGE * 4 + wav_start_point * 4; i++)
    {
        int16_t show = wav.data.block[i];
        // int16_t show = (wav.data.block[i] >> 8) | (wav.data.block[i] << 8);

        if ((i % 4 + 1) == 1)
        {
            wave1234p.ch1[i / 4 - wav_start_point] = show;
        }
        if ((i % 4 + 1) == 2)
        {
            wave1234p.ch2[i / 4 - wav_start_point] = show;
        }
        if ((i % 4 + 1) == 3)
        {
            wave1234p.ch3[i / 4 - wav_start_point] = show;
        }
        if ((i % 4 + 1) == 4)
        {
            wave1234p.ch4[i / 4 - wav_start_point] = show;
        }
    }
    return &wave1234p;
}
Wave1234 *wav_decode::wave_to_chs_8c(bool show_decoded_data)
{

    for (int i = wav_start_point * 8; i < RANGE * 8 + wav_start_point * 8; i++)

    {
        if (((i % 8 + 1) == 1) && (show_decoded_data))
        {

            cout << "###line";
            cout.width(2);
            cout << i / 8 - wav_start_point << "####|||";
        }
        if ((i == 0) && (show_decoded_data))
        {
            printf("|");
        }

        int16_t show = wav.data.block[i];
        // int16_t show = (wav.data.block[i] >> 8) | (wav.data.block[i] << 8);

        if ((i % 8 + 1) == 1)
        {
            wave1234p.ch1[i / 8 - wav_start_point] = show;
        }
        if ((i % 8 + 1) == 2)
        {
            wave1234p.ch2[i / 8 - wav_start_point] = show;
        }
        if ((i % 8 + 1) == 3)
        {
            wave1234p.ch3[i / 8 - wav_start_point] = show;
        }
        if ((i % 8 + 1) == 4)
        {
            wave1234p.ch4[i / 8 - wav_start_point] = show;
        }
        if (((i % 8 + 1) < 5) && (show_decoded_data))
        {
            printf("ch%d:", i % 8 + 1);
            printf("%6d", show);
            printf("|  |");
        }

        if (((i % 8 + 1) == 8) && (show_decoded_data))
        {
            printf("\n");
        }
    }
    return &wave1234p;
}
void wav_decode::set_start_point(int set_value)
{
    wav_start_point = set_value;
}
void wav_decode::set_wav_path(const char *path)
{
    wav_path.assign(path);
}

Wave1234 *wav_decode::hamming()
{
    Wave1234 copy = wave1234p;

    for (int i = 0; i < RANGE; i++)
    {
        double a = 1;
        double b = 0.9375;
        double c = 0;
        if (i > 1)
        {
            wave1234p.ch1[i] = a * copy.ch1[i] - b * copy.ch1[i - 1] - c * copy.ch1[i - 2];
            wave1234p.ch2[i] = a * copy.ch2[i] - b * copy.ch2[i - 1] - c * copy.ch2[i - 2];
            wave1234p.ch3[i] = a * copy.ch3[i] - b * copy.ch3[i - 1] - c * copy.ch3[i - 2];
            wave1234p.ch4[i] = a * copy.ch4[i] - b * copy.ch4[i - 1] - c * copy.ch4[i - 2];
        }

        wave1234p.ch1[i] *= (0.5 - 0.5 * cos(2 * PI * i / (RANGE - 1)));
        wave1234p.ch2[i] *= (0.5 - 0.5 * cos(2 * PI * i / (RANGE - 1)));
        wave1234p.ch3[i] *= (0.5 - 0.5 * cos(2 * PI * i / (RANGE - 1)));
        wave1234p.ch4[i] *= (0.5 - 0.5 * cos(2 * PI * i / (RANGE - 1)));
    }
    return &wave1234p;
}