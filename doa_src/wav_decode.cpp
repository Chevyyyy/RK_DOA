#include"wav_decode.hpp"
#include"iostream"

using namespace std;


void wav_decode::record()
{

    system("arecord -D hw:0,0 -c 8 -r 44100 -s 110   -f  S16_LE  --period-size=1024  --buffer-size=4096 sound_data/test.wav &>a.log");
    // cout << "recording";
    // usleep(500000);
    // system("clear");

}

void wav_decode::wave_to_chs()
{
    for (int i =wav_start_point * 8; i < range * 8 +wav_start_point * 8; i++)

    {
        if (((i % 8 + 1) == 1) && (show_decoded_data))
        {

            cout << "###line";
            cout.width(2);
            cout << i / 8 -wav_start_point << "####|||";
        }
        if ((i == 0) && (show_decoded_data))
        {
            printf("|");
        }
        int16_t show = wav.data.block[i];
        // int16_t show = (wav.data.block[i] >> 8) | (wav.data.block[i] << 8);
        if ((show > 3) || (show < -3))
        {
            // show = 0;
        }

        if ((i % 8 + 1) == 1)
        {
            wave1234.ch1[i / 8 -wav_start_point] = show;
        }
        if ((i % 8 + 1) == 2)
        {
            wave1234.ch2[i / 8 -wav_start_point] = show;
        }
        if ((i % 8 + 1) == 3)
        {
            wave1234.ch3[i / 8 -wav_start_point] = show;
        }
        if ((i % 8 + 1) == 4)
        {
            wave1234.ch4[i / 8 -wav_start_point] = show;
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
}

