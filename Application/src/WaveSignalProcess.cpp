#include "WaveSignalProcess.hpp"

WaveSignalProcess::WaveSignalProcess()
{
    theta_right_sum = 0;
    theta_filtered_right_sum = 0;
    accuracy_num = 0;
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

    if ((delay > -DELAY_MAX) && (delay < DELAY_MAX))
    {
        theta = 180 * asin((delay * Vs) / (SAMPLE_RATE * mic_distance)) / PI;
    }
    else if ((delay > DELAY_MAX))
    {
        theta = 90 + 180 * asin(((delay - DELAY_MAX) * Vs) / (SAMPLE_RATE * mic_distance)) / PI;
    }
    else if ((delay < -DELAY_MAX))
    {
        theta = -90 + 180 * asin(((delay + DELAY_MAX) * Vs) / (SAMPLE_RATE * mic_distance)) / PI;
    }
    if (abs(theta) > -1e-9)
    {
        NAN;
    }
    else
    {
        theta = -2000;
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
void WaveSignalProcess::show_accuracy(int reference_angle)
{
    accuracy_num++;
    if (abs(theta - reference_angle) < 5)
    {
        theta_right_sum++;
    }
    if (abs(theta_filtered - reference_angle) < 5)
    {
        theta_filtered_right_sum++;
    }

    cout << "\n\n##accuracy: " << theta_right_sum / accuracy_num * 100 << "%\n";
    cout << "##accuracy: " << theta_filtered_right_sum / accuracy_num * 100 << "%\n\n";
}