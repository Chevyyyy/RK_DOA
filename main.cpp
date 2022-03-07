#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmath"
#include "unistd.h"
#include "iomanip"
#include "iostream"
#include <cmath>
#include "wav_decode.hpp"
#include "zo-gcc-phat.hpp"
#include "visualize.hpp"
#include "LowPassFilter.hpp"
#include <chrono>
#include "main.hpp"
#include "WaveSignalProcess.hpp"

using namespace chrono;
using namespace std;
double count1 = 0;
double count2 = 0;
double count3 = 0;
double count4 = 0;
double count5 = 0;
double count6 = 0;
double num = 0;
int main()
{
    cout << "let's start!" << endl;

    // wav decode object init
    wav_decode wav_decoder;
    wav_decoder.set_start_point(10);

    // GCCPHAT object init
    zo::GccPhat *gcc_phat = zo::GccPhat::create();
    gcc_phat->init(RANGE);

    // visualize object init
    visualize vis_tool;
    vis_tool.visualize_qt();

    // wave signal processing object init
    WaveSignalProcess SP_tool(LPF_CUTOFF, DELTA);

    for (int i = 0; i < 10000; i++)
    {
        num++;
        // timer start
        auto start = system_clock::now();

        wav_decoder.set_wav_path("../sound_data/labelled_dataset/30s_90deg_cafe_restaurant.wav");
        // wav_decoder.record();
        wav_decoder.read_wav_file();
        wav_decoder.set_start_point(i);

        Wave1234 *wavech1234 = wav_decoder.wave_to_chs(SHOW_RAW_DATA);

        // get the tau and filter it with LPF
        SP_tool.delay1234.delay12.delay = gcc_phat->execute(wavech1234->ch1, wavech1234->ch2, 7);
        SP_tool.delay1234.delay13.delay = gcc_phat->execute(wavech1234->ch1, wavech1234->ch3, 14) / 2.0;
        SP_tool.delay1234.delay14.delay = gcc_phat->execute(wavech1234->ch1, wavech1234->ch4, 21) / 3.0;
        SP_tool.delay1234.delay23.delay = gcc_phat->execute(wavech1234->ch2, wavech1234->ch3, 7);
        SP_tool.delay1234.delay24.delay = gcc_phat->execute(wavech1234->ch2, wavech1234->ch4, 14) / 2.0;
        SP_tool.delay1234.delay34.delay = gcc_phat->execute(wavech1234->ch3, wavech1234->ch4, 7);
        // SP_tool.limit_delay_range();
        SP_tool.show_delay();

        //for accuracy calculation
        if ((SP_tool.delay1234.delay12.delay  <5.1) && (SP_tool.delay1234.delay12.delay > 3.9))
        {
            count1++;
        }
        if ((SP_tool.delay1234.delay13.delay < 5.1) && (SP_tool.delay1234.delay13.delay >3.9))
        {
            count2++;
        }
        if ((SP_tool.delay1234.delay14.delay <5.1) && (SP_tool.delay1234.delay14.delay >3.9))
        {
            count3++;
        }
        if ((SP_tool.delay1234.delay23.delay <5.1) && (SP_tool.delay1234.delay23.delay >3.9))
        {
            count4++;
        }
        if ((SP_tool.delay1234.delay24.delay <5.1) &&(SP_tool.delay1234.delay24.delay >3.9))
        {
            count5++;
        }
        if ((SP_tool.delay1234.delay34.delay <5.1) && (SP_tool.delay1234.delay34.delay >3.9))
        {
            count6++;
        }

        // filter
        double delay = SP_tool.get_delay_from_6_data();
        double theta = SP_tool.get_theta();
        double theta_filtered = SP_tool.update(theta);
        

        // calculate the volume of a period of sound
         double volume = SP_tool.get_volume(wavech1234->ch1);

        // visualize
         vis_tool.write_angles_to_txt(theta,theta_filtered);

        // timer end
        auto end = system_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        double duration_time = double(duration.count()) * microseconds::period::num / microseconds::period::den;

        // print
        //  cout <<left<<setw(7)<< "theta: " <<left<<setw(15)<<theta<<left<<setw(7)<< "filtered: " <<left<<setw(15)<<theta_filtered<< left << setw(13) << "sample_num:" << i << left << setw(13) << "   sample_fre: "<< 1/duration_time<<"Hz"<<left<<setw(7)<< "  volume: " <<left<<setw(15)<<volume<<endl;

        usleep(500);
    }
    cout << "accuarcy12:" << count1/ num<< "  ";
    cout << "accuarcy13:" << count2 / num<< "  ";
    cout << "accuarcy14:" << count3 / num<< "  ";
    cout << "accuarcy23:" << count4 / num<< "  ";
    cout << "accuarcy14:" << count5 / num<< "  ";
    cout << "accuarcy34:" << count6 / num<< "  ";
}
