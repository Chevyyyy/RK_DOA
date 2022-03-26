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

    // if there is a obvious sound count
    double no_obvious_sound_count = 0;

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
    double R_init = 10;
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
    R << R_init;
    P << 1, 1, 1, 1;

    KalmanFilter KF(dt, A, C, Q, R, P);

    // Best guess of initial states
    Eigen::VectorXd x0(n);
    double t = 0;
    x0 << 0, 0;
    KF.init(t, x0);

#ifndef ON_RKCHIP_FLAG
    wav_decoder.set_wav_path(train_path);
#endif

    int k = 0;
    double sum = 0;
    double sum_filtered = 0;
    double white_cc = 0;
    int deg_0_count = 0;
    for (int i = 0; i < 100000; i++)
    {
        // timer start
        auto start = system_clock::now();
        double volume = -1;
        SP_tool.theta = -20;
        double target_band_amplitued = 0;
        double confidence_CC = 0;

        no_obvious_sound_count = 0;

        Wave1234 *wavech1234;

        //@1st STEP measurement confidence and data select
        while (volume < VOLUME_THRESHOLD)
        {
#ifndef ON_RKCHIP_FLAG
            wav_decoder.set_start_point((i + k) * RANGE + 43620);
#else
            wav_decoder.record();
#endif
            k++;
            wav_decoder.read_wav_file();
            // calculate the volume of a period of sound
            wavech1234 = wav_decoder.wave_to_chs(SHOW_RAW_DATA);
            // usleep(100000);
            volume = SP_tool.get_volume(wavech1234->ch1);


#ifdef PHAT_SPR
            // PHAT-SPR
            double delay_cc_white[3];
            gcc_phat->PHAT_SRP_3mic(wavech1234, 5, delay_cc_white, white_cc);
            double delay = delay_cc_white[0];
            confidence_CC = delay_cc_white[1];
            white_cc = delay_cc_white[2];
#endif

            // get the theta through delays
            SP_tool.get_theta(delay);

            Eigen::VectorXd y(m);

            t += dt;
            if (SP_tool.theta != -2000) // if theta is NAN, do not pass it to kalman filter
            {
                if (SP_tool.theta == 0)
                {
                    deg_0_count++;
                }
                else
                {
                    deg_0_count = 0;
                }
                if ((SP_tool.theta == 0) && (deg_0_count > 2))
                {
                    y << 0;
                }
                else if (SP_tool.theta != 0)
                {
                    y << SP_tool.theta;
                }
                //update the deviation of measurment data
                R << R_init / confidence_CC;
            }
            no_obvious_sound_count++;
            if (delay != -20)
            {
                KF.update(y, dt, A, R);
            }
            else
            {
                volume = -20;
            }
            if (no_obvious_sound_count > no_obvious_count_threshold)
            {
                vis_tool.write_angles_to_txt(-1000, -1000);
                cout << "&&&&&&&&&&&&&&&&&&&&&\n****************\n";
            }
            cout << "\nwhite_cc$$$$$$$: " << white_cc << endl;
        }

        //@2nd STEP choosen data feeds kalman model

        SP_tool.theta_filtered = (KF.state())[0];

        // for accuracy calculation
        SP_tool.show_accuracy(20, i + 1);

        cout << "confidence_CC$$$$$$$: " << confidence_CC << endl;

        // visualize
        vis_tool.write_angles_to_txt(SP_tool.theta, SP_tool.theta_filtered);

        // timer end
        auto end = system_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        double duration_time = double(duration.count()) * microseconds::period::num / microseconds::period::den;

        // print
        cout << left << setw(7) << "theta@@: " << left << setw(12) << SP_tool.theta << left << setw(7) << "filtered: " << left << setw(12) << SP_tool.theta_filtered << left << setw(5) << "num:" << i << left << setw(13) << "   sample_fre: " << 1 / duration_time << "Hz\n";

        //@ 3rd STEP update kalman due to time interval
        A(0, 1) = duration_time;
        dt = duration_time;
    }
}
