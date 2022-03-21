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
#include "kalman.hpp"
// #include "MUSIC.hpp"

using namespace chrono;
using namespace std;
int main()
{
    cout << "let's start!" << endl;

    // wav decode object init
    wav_decode wav_decoder;
    wav_decoder.set_start_point(10);

    // GCCPHAT object init
    zo::GccPhat *gcc_phat = zo::GccPhat::create();
    gcc_phat->init(RANGE);

    // MUSIC init
    //  MUSIC music_doa(4, 1, RANGE, mic_distance);

    // visualize object init
    visualize vis_tool;
    vis_tool.visualize_qt();

    // wave signal processing object init
    WaveSignalProcess SP_tool;

    // LPF init
    LowPassFilter LPF(LPF_CUTOFF, DELTA);

    // kalman init
    int n = 2; // Number of states
    int m = 1; // Number of measurements

    double dt = 1 / 4.5; // Time step

    Eigen::MatrixXd A(n, n); // System dynamics matrix
    Eigen::MatrixXd C(m, n); // Output matrix
    Eigen::MatrixXd Q(n, n); // Process noise covariance
    Eigen::MatrixXd R(m, m); // Measurement noise covariance
    Eigen::MatrixXd P(n, n); // Estimate error covariance

    // Discrete LTI projectile motion, measuring position only
    A << 1, dt, 0, 1;
    C << 1, 0;

    // Reasonable covariance matrices
    Q << 0.1, 0, 0, 0.1;
    R << 10;
    P << 1, 1, 1, 1;

    KalmanFilter KF(dt, A, C, Q, R, P);

    // Best guess of initial states
    Eigen::VectorXd x0(n);
    double t = 0;
    x0 << 0, 0;
    KF.init(t, x0);

#ifndef ON_RKCHIP_FLAG
    wav_decoder.set_wav_path("../sound_data/labelled_dataset/30s_20deg_speech.wav");
#endif

    int k = 0;
    double sum = 0;
    for (int i = 0; i < 100000; i++)
    {
        // timer start
        auto start = system_clock::now();
        double volume = -1;

        Wave1234 *wavech1234;
        while (volume < VOLUME_THRESHOLD)
        {
#ifndef ON_RKCHIP_FLAG
            wav_decoder.set_start_point((i + k) * RANGE);
#else
            wav_decoder.record();
#endif
            k++;
            wav_decoder.read_wav_file();
            // calculate the volume of a period of sound
            wavech1234 = wav_decoder.wave_to_chs(SHOW_RAW_DATA);
            // usleep(100000);
            volume = SP_tool.get_volume(wavech1234->ch1);
        }

        // music_doa.update(wavech1234);

        // get the tau and cc
        int delay_cc_p[2];
        gcc_phat->execute(wavech1234->ch1, wavech1234->ch2, 7, delay_cc_p);
        SP_tool.delay1234.delay12.delay = delay_cc_p[0];
        SP_tool.delay1234.delay12.cc = delay_cc_p[1];

        int delay_cc_p1[2];
        gcc_phat->execute(wavech1234->ch1, wavech1234->ch3, 14, delay_cc_p1);
        SP_tool.delay1234.delay13.delay = delay_cc_p1[0] / 2.0;
        SP_tool.delay1234.delay13.cc = delay_cc_p1[1];

        int delay_cc_p2[2];
        gcc_phat->execute(wavech1234->ch1, wavech1234->ch4, 21, delay_cc_p2);
        SP_tool.delay1234.delay14.delay = delay_cc_p2[0] / 3.0;
        SP_tool.delay1234.delay14.cc = delay_cc_p2[1];

        int delay_cc_p3[2];
        gcc_phat->execute(wavech1234->ch2, wavech1234->ch3, 7, delay_cc_p3);
        SP_tool.delay1234.delay23.delay = delay_cc_p3[0];
        SP_tool.delay1234.delay23.cc = delay_cc_p3[1];

        int delay_cc_p4[2];
        gcc_phat->execute(wavech1234->ch2, wavech1234->ch4, 7, delay_cc_p4);
        SP_tool.delay1234.delay24.delay = delay_cc_p4[0] / 2.0;
        SP_tool.delay1234.delay24.cc = delay_cc_p4[1];

        int delay_cc_p5[2];
        gcc_phat->execute(wavech1234->ch3, wavech1234->ch4, 7, delay_cc_p5);
        SP_tool.delay1234.delay34.delay = delay_cc_p5[0];
        SP_tool.delay1234.delay34.cc = delay_cc_p5[1];

        // SP_tool.show_delay();

        // filter
        double delay = SP_tool.get_delay_from_6_data();
        double theta = SP_tool.get_theta();

        Eigen::VectorXd y(m);

        t += dt;
        y << theta;
        if (delay != -20)
        {
            KF.update(y, dt, A);
        }

        double theta_filtered = (KF.state())[0];

        // for accuracy calculation

        if (abs(theta - 20) < 3)
        {
            sum++;
        }
        cout << "\n\n##accuracy: " << sum / i*100<<"%\n\n";

        // visualize
        vis_tool.write_angles_to_txt(theta, theta_filtered);

        // timer end
        auto end = system_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        double duration_time = double(duration.count()) * microseconds::period::num / microseconds::period::den;

        // print
        cout << left << setw(7) << "theta: " << left << setw(12) << theta << left << setw(7) << "filtered: " << left << setw(12) << theta_filtered << left << setw(5) << "num:" << i << left << setw(13) << "   sample_fre: " << 1 / duration_time << "Hz" << left << setw(7) << "  volume: " << left << setw(15) << volume << "%" << endl;
        A(0, 1) = duration_time;
        dt = duration_time;
    }
}
