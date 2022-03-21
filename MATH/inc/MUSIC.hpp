#ifndef __MUSIC__
#define __MUSIC__
#include "wav_decode.hpp"
#include "Eigen/Dense"
#include "numeric"
#include "Eigen/Eigen"
#include "iostream"
#include "main.hpp"
#include "complex"
#include "algorithm"
using namespace Eigen;
class MUSIC
{
private:
    /* data */
    int mic_num;
    int source_num;
    int frame_range;
    int mic_dd;
    Wave1234 wave1234;

public:
    MUSIC(/* args */);
    MUSIC(int mic_num1, int source_num1, int frame_range1, int mic_distance1);
    ~MUSIC();
    void update(Wave1234* ch4_wave_mic);
};

#endif