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
#include"LowPassFilter.hpp"
#include<chrono>

#define LPF_CUTOFF 100

using namespace chrono;
using namespace std;

int main()
{
    cout << "let's start!"<< endl;


//wav decode object init
    wav_decode wav_decoder;
    wav_decoder.set_start_point(10);


//GCCPHAT object init  
    zo::GccPhat *gcc_phat = zo::GccPhat::create();
    gcc_phat->init(RANGE);

//visualize object init
    visualize vis_tool;
    vis_tool.visualize_qt();

//Low pass filter object init
    LowPassFilter LPfilter_tool(LPF_CUTOFF,DELTA);



    for (int i = 0; i < 10000;i++)
    {
        // timer start
        auto start = system_clock::now();


        wav_decoder.record();
        wav_decoder.read_wav_file();


        Wave1234* wavech1234=wav_decoder.wave_to_chs(0);

        //get the tau and filter it with LPF
        double delay = gcc_phat->execute(wavech1234->ch1, wavech1234->ch2, 10);
        if(delay>DELAY_MAX){
            delay = DELAY_MAX;
        }
        else if(delay<(-DELAY_MAX))
        {
            delay = -DELAY_MAX;
        }

        //filter
        double delay_filtered=LPfilter_tool.update(delay);
        double theta_filtered = DELAY_TO_THETA(delay_filtered);
        double theta=DELAY_TO_THETA(delay);
        

        //visualize
        // vis_tool.visualize_terminal(theta);
        vis_tool.write_angles_to_txt(theta,theta_filtered);


        //timer end
        auto end = system_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        double duration_time = double(duration.count()) * microseconds::period::num / microseconds::period::den;

        //print
        cout <<left<<setw(7)<< "theta: " <<left<<setw(15)<<theta<<left<<setw(7)<< "filtered: " <<left<<setw(15)<<theta_filtered<< left << setw(13) << "sample_num:" << i << left << setw(13) << "   sample_fre: "<< 1/duration_time<<"Hz"<<endl;

        usleep(500);   
    }
}
