#include "WaveSignalProcess.hpp"

WaveSignalProcess::WaveSignalProcess()
{
}
double WaveSignalProcess::get_delay_from_6_data()
{
    vector<double> cc = {delay1234.delay12.cc, delay1234.delay13.cc, delay1234.delay14.cc, delay1234.delay23.cc, delay1234.delay24.cc, delay1234.delay34.cc};

    vector<double> delay = {delay1234.delay12.delay, delay1234.delay13.delay, delay1234.delay14.delay, delay1234.delay23.delay, delay1234.delay24.delay, delay1234.delay34.delay};

    // sort(delay.begin(), delay.end());
    sort(cc.begin(), cc.end());
    double mean_delay = accumulate(delay.begin() + 1, delay.end() - 1, decltype(delay)::value_type(0.0)) / 4.0;
    double std_delay = 0;
    for (int i = 0; i < 6; i++)
    {
        cc[i] = 10 * log10(cc[i]);
    }

    double power_cc = accumulate(cc.begin() + 2, cc.end() - 2, decltype(delay)::value_type(0.0)) / 2;

    for (int i = 0; i < 4; i++)
    {
        std_delay += (delay[i + 1] - mean_delay) * (delay[i + 1] - mean_delay);
    }
    std_delay = sqrt(std_delay / 6);

    if ((std_delay > 10) || (power_cc < 20))
    {
        return -20;
    }

    std::cout << "std: " << std_delay << "  ";
    std::cout << "dB: " << power_cc << "  ";
    mean_delay = (delay[0] + delay[1] + delay[3]) / 3;
    all_6_data_delay = mean_delay;

    return mean_delay;
}
double WaveSignalProcess::get_delay_from_3_data()
{
    vector<double> cc = {delay1234.delay12.cc, delay1234.delay13.cc, delay1234.delay23.cc};

    vector<double> delay = {delay1234.delay12.delay, delay1234.delay13.delay, delay1234.delay23.delay};

    sort(delay.begin(), delay.end());
    sort(cc.begin(), cc.end());
    double mean_delay = accumulate(delay.begin(), delay.end(), decltype(delay)::value_type(0.0)) / 3.0;
    double std_delay = 0;
    for (int i = 0; i < 3; i++)
    {
        cc[i] = 10 * log10(cc[i]);
    }

    double power_cc = accumulate(cc.begin(), cc.end(), decltype(delay)::value_type(0.0)) / 3;

    for (int i = 0; i < 3; i++)
    {
        std_delay += (delay[i] - mean_delay) * (delay[i] - mean_delay);
    }
    std_delay = sqrt(std_delay / 3);

    // if ((std_delay > 3) || (power_cc < 30))
    // {
    //     return -20;
    // }

    std::cout << "std: " << std_delay << "  ";
    std::cout << "dB: " << power_cc << "  ";
    all_6_data_delay = mean_delay;

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
double WaveSignalProcess::get_theta(double delay)
{
    double theta = 0;
    if ((delay > -DELAY_MAX) && (delay < DELAY_MAX))
    {
        theta = 180 * asin((delay * Vs) / (SAMPLE_RATE * mic_distance)) / PI;
        if (abs((delay * Vs) / (SAMPLE_RATE * mic_distance)) > 1)
        {
            return -2000;
        }
    }
    else if ((delay > DELAY_MAX))
    {
        theta = 90 + 180 * asin(((delay - DELAY_MAX) * Vs) / (SAMPLE_RATE * mic_distance)) / PI;
                if (abs(((delay - DELAY_MAX) * Vs) / (SAMPLE_RATE * mic_distance)) > 1)
        {
            return -2000;
        }
    }
    else if ((delay < -DELAY_MAX))
    {
        theta = -90 + 180 * asin(((delay + DELAY_MAX) * Vs) / (SAMPLE_RATE * mic_distance)) / PI;
                if (abs(((delay + DELAY_MAX) * Vs) / (SAMPLE_RATE * mic_distance)) > 1)
        {
            return -2000;
        }
    }

    return theta;
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
