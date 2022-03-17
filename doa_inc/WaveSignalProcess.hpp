#ifndef  _WAVE_SIGNAL_PROCESS_
#define  _WAVE_SIGNAL_PROCESS_
#include "LowPassFilter.hpp"
#include<vector>
#include"main.hpp"
#include "math.h"
#include "iostream"
#include "iomanip"
#include "algorithm"
#include "numeric"
using namespace std;

typedef struct Delayxx_format
{
    double delay;
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


class WaveSignalProcess : public LowPassFilter
{
private:
    
public:
    WaveSignalProcess(float iCutOffFrequency, float iDeltaTime);
    WaveSignalProcess();
    ~WaveSignalProcess(){};
    double get_volume(std::vector<int16_t> &wav_vector);
    void show_delay();
    double get_delay_from_6_data();
    Delay1234 delay1234;
    double get_theta();

private:
    double all_6_data_delay;
};

#endif