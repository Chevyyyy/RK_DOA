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




using namespace std;

int main()
{


//wav decode object init
    wav_decode wav_decoder;
    // wav_decoder.record();
    wav_decoder.read_wav_file();

//GCCPHAT object init  
    zo::GccPhat *gcc_phat = zo::GccPhat::create();
    gcc_phat->init(RANGE);

//visualize object init
    visualize vis_tool;

//Low pass filter object init
    LowPassFilter LPfilter_tool(50,DELTA);
 

    for (int i = 0; i < 10000;i++)
    {
        wav_decoder.set_start_point(i);
        Wave1234* wavech1234=wav_decoder.wave_to_chs(0);

        //get the tau and filter it with LPF
        int delay = gcc_phat->execute(wavech1234->ch1, wavech1234->ch2, 10);
        double delay_filtered=LPfilter_tool.update(delay);
        double theta = DELAY_TO_THETA(delay_filtered);
        
        //visualize
        vis_tool.visualize_terminal(theta);
        usleep(5000);   
    }
}
