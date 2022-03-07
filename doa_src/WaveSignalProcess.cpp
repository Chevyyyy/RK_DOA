#include "WaveSignalProcess.hpp"



WaveSignalProcess::WaveSignalProcess() 
{
    
}
WaveSignalProcess::WaveSignalProcess(float iCutOffFrequency, float iDeltaTime) : 
LowPassFilter(iCutOffFrequency, iDeltaTime)
{
    
}
double WaveSignalProcess::get_delay_from_6_data()
{
    vector<double> v = {delay1234.delay12.delay, delay1234.delay13.delay, delay1234.delay14.delay, delay1234.delay23.delay, delay1234.delay24.delay, delay1234.delay34.delay};
    sort(v.begin(), v.end());
    all_6_data_delay=accumulate(v.begin() + 1, v.end() - 1,decltype(v)::value_type(0.0))/4;
    return all_6_data_delay;
}

void WaveSignalProcess::show_delay()
{
    cout <<left<<setw(10)<< delay1234.delay12.delay;
    cout <<left<<setw(10)<< delay1234.delay13.delay;
    cout <<left<<setw(10)<< delay1234.delay14.delay;
    cout <<left<<setw(10)<< delay1234.delay23.delay;
    cout <<left<<setw(10)<< delay1234.delay24.delay;
    cout <<left<<setw(10)<< delay1234.delay34.delay << endl;
}
double WaveSignalProcess::get_theta()
{
    if((all_6_data_delay>-DELAY_MAX)&&(all_6_data_delay<DELAY_MAX))
    {
        return 180 * asin((all_6_data_delay * Vs) / (44100 * mic_distance)) / 3.1415926;
    }
    else if((all_6_data_delay>DELAY_MAX))
    {
        return 90+180 * asin(((all_6_data_delay-DELAY_MAX) * Vs) / (44100 * mic_distance)) / 3.1415926;
    }
    else if((all_6_data_delay<-DELAY_MAX))
    {
        return -90+180 * asin(((all_6_data_delay+DELAY_MAX) * Vs) / (44100 * mic_distance)) / 3.1415926;
    }
}
double WaveSignalProcess::get_volume(std::vector<int16_t> &wav_vector)
{
    double sum = 0;
    for (int i = 0; i < RANGE; i++)
    {
        sum += fabs(wav_vector[i]);
    }
    return sum / RANGE;
}
