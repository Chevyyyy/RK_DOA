#include "WaveSignalProcess.hpp"

WaveSignalProcess::WaveSignalProcess()
{
}
double WaveSignalProcess::get_delay_from_6_data()
{
    vector<double> cc = {delay1234.delay12.cc, delay1234.delay13.cc, delay1234.delay14.cc, delay1234.delay23.cc, delay1234.delay24.cc, delay1234.delay34.cc};

    vector<double> delay = {delay1234.delay12.delay, delay1234.delay13.delay, delay1234.delay14.delay, delay1234.delay23.delay, delay1234.delay24.delay, delay1234.delay34.delay};

    sort(delay.begin(), delay.end());
    sort(cc.begin(), cc.end());
    double mean_delay = accumulate(delay.begin() + 1, delay.end() - 1, decltype(delay)::value_type(0.0)) / 4.0;
    double std_delay = 0;
    for (int i = 0; i < 4; i++)
    {
        std_delay += (delay[i + 1] - mean_delay) * (delay[i + 1] - mean_delay);
    }
    std_delay = sqrt(std_delay);

    std::cout << "std: " << std_delay << "  ";

    for (int i = 0; i < 6; i++)
    {
        cc[i] = 10 * log10(cc[i]);
    }

    double power_cc = accumulate(cc.begin() + 1, cc.end() - 1, decltype(delay)::value_type(0.0)) / 4;
    std::cout << "dB: " << power_cc << "  ";

    all_6_data_delay = mean_delay;
    if ((std_delay > 1.5)||(power_cc<39))
    {
        return -20;
    }

    return mean_delay;
}

void WaveSignalProcess::show_delay()
{
    cout << left << setw(10) << delay1234.delay12.delay;
    cout << left << setw(10) << delay1234.delay13.delay;
    cout << left << setw(10) << delay1234.delay14.delay;
    cout << left << setw(10) << delay1234.delay23.delay;
    cout << left << setw(10) << delay1234.delay24.delay;
    cout << left << setw(10) << delay1234.delay34.delay << endl;
}
double WaveSignalProcess::get_theta()
{
    if ((all_6_data_delay > -DELAY_MAX) && (all_6_data_delay < DELAY_MAX))
    {
        return 180 * asin((all_6_data_delay * Vs) / (SAMPLE_RATE * mic_distance)) / 3.1415926;
    }
    else if ((all_6_data_delay > DELAY_MAX))
    {
        return 90 + 180 * asin(((all_6_data_delay - DELAY_MAX) * Vs) / (SAMPLE_RATE * mic_distance)) / 3.1415926;
    }
    else if ((all_6_data_delay < -DELAY_MAX))
    {
        return -90 + 180 * asin(((all_6_data_delay + DELAY_MAX) * Vs) / (SAMPLE_RATE * mic_distance)) / 3.1415926;
    }
}
double WaveSignalProcess::get_volume(std::vector<float> &wav_vector)
{
    double sum = 0;
    for (int i = 0; i < RANGE; i++)
    {
        sum += fabs(wav_vector[i]) / 32767;
    }
    return sum / RANGE * 100;
}
