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
    wav_decoder.set_start_point(0);

    // GCCPHAT object init
    zo::GccPhat *gcc_phat = zo::GccPhat::create();
    gcc_phat->init(RANGE);

    // MUSIC init
    //  MUSIC music_doa(4, 1, RANGE, mic_distance);

    // if there is a obvious sound count
    double no_obvious_sound_count = no_obvious_count_threshold;
    double obvious_sound_count = 0;

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
    double R_init = 50;
    double dt = 1 / 4.5; // Time step

    Eigen::MatrixXd A(n, n); // System dynamics matrix
    Eigen::MatrixXd C(m, n); // Output matrix
    Eigen::MatrixXd Q(n, n); // Process noise covariance
    Eigen::MatrixXd R(m, m); // Measurement noise covariance
    Eigen::MatrixXd P(n, n); // Estimate error covariance
    Eigen::VectorXd y(m);    // measurments

    // Discrete LTI projectile motion, measuring position only
    A << 1, dt, 0, 1;
    C << 1, 0;

    // Reasonable covariance matrices
    Q << 0, 0, 0, 0.01;
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
        double volume = -1;
        SP_tool.theta = -20;
        double target_band_amplitued = 0;
        double confidence_CC = 0;
        double speech_ratio = 0;
        double possibility = 0;

        Wave1234 *wavech1234;

        //@1st STEP measurement confidence and data select
        while (volume < 0)
        {
            auto start = system_clock::now();
            
#ifndef ON_RKCHIP_FLAG
            wav_decoder.set_start_point((i + k) * RANGE  + 43620);
            usleep(Usleep_time);
#else
            wav_decoder.record();
#endif
            k++;
            wav_decoder.read_wav_file();
            // calculate the volume of a period of sound
            wav_decoder.wave_to_chs(SHOW_RAW_DATA);
            wavech1234 = wav_decoder.hamming();
            volume = SP_tool.get_volume(wavech1234->ch1);

#ifdef PHAT_SPR
            // PHAT-SPR
            double delay_cc_white_second_cc_ratio[6];
            gcc_phat->PHAT_SRP_4mic(wavech1234, 5, delay_cc_white_second_cc_ratio, 0);
            double delay = delay_cc_white_second_cc_ratio[0];

            confidence_CC = delay_cc_white_second_cc_ratio[1];
            speech_ratio = delay_cc_white_second_cc_ratio[5] * 100;

            // if there is no sound source at 0deg, regraded the cc at 0deg as white noise

#endif
            // no obvious sound detect
            if (no_obvious_sound_count > no_obvious_count_threshold)
            {
                vis_tool.write_angles_to_txt(-1000, -1000);
                cout << "&&&&&&&&&&&&&&&&&&&&&\n****************\n";
                if (obvious_sound_count == 0)
                {
                    SP_tool.theta_filtered = SP_tool.get_theta(delay);

                    KF.x_hat(0, 0) = SP_tool.theta_filtered;
                }
                if (obvious_sound_count > obvious_strict_sequence)
                {
                    obvious_sound_count = 0;
                }
            }
            // update the deviation of measurment data
            R << R_init;

            // get the theta through delays
            SP_tool.get_theta(delay);
            possibility = KF.Possiblity_of_coherent_source(SP_tool.theta)*100;

            if ((possibility < single_measure_tolerance) || (delay == -20) || (delay == 0))
            {

                if (delay_cc_white_second_cc_ratio[3] != -20)
                {
                    delay = delay_cc_white_second_cc_ratio[3];
                    SP_tool.get_theta(delay);
                    possibility = KF.Possiblity_of_coherent_source(SP_tool.theta)*100;

                    if (possibility < single_measure_tolerance)
                    {
                        delay = -20;
                        KF.R << 1e10;
                        SP_tool.theta = -120;
                        KF.C(0, 1) *= speed_attenuation_ratio;
                        obvious_sound_count = 0;
                    }
                }
                else
                {
                    delay = -20;
                    KF.R << 1e10;
                    SP_tool.theta = -120;
                    KF.C(0, 1) *= speed_attenuation_ratio;

                    obvious_sound_count = 0;
                }
            }


            if (delay != 0)
            {
                white_cc = delay_cc_white_second_cc_ratio[2];
                deg_0_count = 0;
            }
            else
            {
                deg_0_count++;
                if (deg_0_count < 1)
                {
                    delay = -20;
                    KF.R << 1e10;
                    SP_tool.theta = -120;
                    KF.C(0, 1) *= speed_attenuation_ratio;
                    obvious_sound_count = 0;
                }
            }
            if (delay != -20)
            {
                obvious_sound_count++;
            }

#ifdef Track_speech_flag
            if ((speech_ratio < speech_ratio_threshold + 10) && (obvious_sound_count < obvious_strict_sequence))
            {
                delay = -20;
                KF.R << 1e10;
                SP_tool.theta = -120;
                KF.C(0, 1) *= speed_attenuation_ratio;
                obvious_sound_count = 0;
            }
            else if ((speech_ratio < speech_ratio_threshold) && (obvious_sound_count > obvious_strict_sequence - 1))
            {
                delay = -20;
                KF.R << 1e10;
                SP_tool.theta = -120;
                KF.C(0, 1) *= speed_attenuation_ratio;

                obvious_sound_count = 0;
            }
#endif

            if ((no_obvious_sound_count > no_obvious_count_threshold) && (obvious_sound_count < obvious_strict_sequence))
            {
                delay = -20;
                KF.R << 1e10;
                SP_tool.theta = -120;
                KF.C(0, 1) *= speed_attenuation_ratio;

                no_obvious_sound_count = no_obvious_count_threshold + 2;
            }
            if (obvious_sound_count > obvious_strict_sequence - 1)
            {
                no_obvious_sound_count = 0;
            }

            t += dt;
            if (SP_tool.theta != -2000) // if theta is NAN, do not pass it to kalman filter
            {
                y << SP_tool.theta;
            }
            no_obvious_sound_count++;

            KF.update(y, dt, A, R);

            if (delay == -20)
            {
                volume = -20;
            }

            // cout << "\nwhite_cc$$$$$$$: " << white_cc << endl;

            //@2nd STEP choosen data feeds kalman model

            SP_tool.theta_filtered = (KF.state())[0];

            // timer end
            auto end = system_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            double duration_time = double(duration.count()) * microseconds::period::num / microseconds::period::den;
            //@ 3rd STEP update kalman due to time interval
            A(0, 1) = duration_time;
            dt = duration_time;

            if (no_obvious_sound_count < no_obvious_count_threshold)
            {
// for accuracy calculation
#ifndef ON_RKCHIP_FLAG
                SP_tool.show_accuracy(refernce_angle);
#endif
                // visualize
                vis_tool.write_angles_to_txt(SP_tool.theta, SP_tool.theta_filtered);
                // print
                cout << left << setw(7) << "theta@@: " << left << setw(7) << fixed << setprecision(2) << SP_tool.theta << left << setw(7) << "filtered: " << left << setw(8) << fixed << setprecision(2) << SP_tool.theta_filtered << left << setw(5) << "num:" << SP_tool.accuracy_num << left << setw(13) << "   sample_fre: " << left << setw(11) << fixed << setprecision(2) << 1 / duration_time << "Hz";
                cout << left << setw(7) << "   CC: " << left << setw(9) << fixed << setprecision(2) << confidence_CC << left << setw(15) << "Speech_Ratio: " << left << setw(7) << fixed << setprecision(2) << speech_ratio << left << setw(10) << "coherent_p: " << possibility << endl;
            }
        }
    }
}
