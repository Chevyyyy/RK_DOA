#ifndef _WAVE_SIGNAL_PROCESS_
#define _WAVE_SIGNAL_PROCESS_
#include "LowPassFilter.hpp"
#include <vector>
#include "main.hpp"
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

class WaveSignalProcess
{
private:
public:
    WaveSignalProcess();
    ~WaveSignalProcess(){};
    double get_volume(std::vector<float> &wav_vector);
    void show_delay();
    Delay1234 delay1234;
    double get_theta(double delay);
    void show_accuracy(int reference_angle);
    double theta;
    double theta_filtered;
    float accuracy_num;

private:
    float theta_right_sum;
    float theta_filtered_right_sum;
};

#endif