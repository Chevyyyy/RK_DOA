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
#include <chrono>
#include "main.hpp"
#include "WaveSignalProcess.hpp"
#include "kalman.hpp"
// #include "MUSIC.hpp"
double confidence_CC_THRESHOLD;
int no_obvious_count_threshold;
double speech_ratio_threshold;
double single_measure_tolerance;
int obvious_strict_sequence;
double speed_attenuation_ratio;
double traking_speed_amplification;
double volume_threshold;
double Passband_Low;
double Passband_High;
double reference_angle;
string train_path;

using namespace chrono;
using namespace std;
int main()
{
    cout << "let's start!" << endl;

    // config through RK_config
    rk_config();

    // wav decode object init
    wav_decode wav_decoder;
    wav_decoder.set_start_point(0);

    // alsa record init
    // AlsaCaptureAudio alsaCaptureAudio;

    // GCCPHAT object init
    zo::GccPhat *gcc_phat = zo::GccPhat::create();
    gcc_phat->init(RANGE, Passband_Low, Passband_High);

    // if there is a obvious sound count
    double no_obvious_sound_count = no_obvious_count_threshold;
    double obvious_sound_count = 0;

    // visualize object init
    visualize vis_tool;
    vis_tool.visualize_qt();

    // wave signal processing object init
    WaveSignalProcess SP_tool;

    // kalman init
    int n = 2; // Number of states
    int m = 1; // Number of measurements
    double R_init = 50;
    double dt = 1.0 / 90.0; // Time step

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
    wav_decoder.set_wav_path(train_path.data());
#endif

    int k = 0;
    double sum = 0;
    double sum_filtered = 0;
    double white_cc = 0;
    int deg_0_count = 0;
    for (int i = 0; i < 10000000; i++)
    {
        // if (i % 3000 == 0)
        // {
        //     system("clear");
        // }
        // timer start
        double volume = -1;
        SP_tool.theta = -20;
        double target_band_amplitued = 0;
        double confidence_CC = 0;
        double speech_ratio = 0;
        double possibility = 0;
        double theta_output = 0;

        Wave1234 *wavech1234;
        Wave1234 *wavech1234_no_window;

        //@1st STEP measurement confidence and data select
        while (volume < volume_threshold)
        {
            if (abs(volume + 1) > 1e-3)
            {
                k++;
            }
            auto start = system_clock::now();

#ifndef ON_RKCHIP_FLAG
            int start_point = (i + k) * RANGE + SAMPLE_RATE * 0;
            if ((start_point > SAMPLE_RATE * 20 - 512))
            {
                exit(1);
            }
            wav_decoder.set_start_point(start_point);
            usleep(Usleep_time);
            wav_decoder.read_wav_file();
            // calculate the volume of a period of sound
            wavech1234_no_window = wav_decoder.wave_to_chs_4c(SHOW_RAW_DATA);
#else
            wavech1234_no_window = wav_decoder.record();
#endif
            wavech1234 = wav_decoder.hamming();

            volume = SP_tool.get_volume(wavech1234_no_window->ch1);
            double volume1 = volume;

#ifdef PHAT_SPR
            // PHAT-SPR
            double delay_cc_white_second_cc_ratio[6];

            gcc_phat->PHAT_SRP_4mic(wavech1234_no_window, wavech1234, 5, delay_cc_white_second_cc_ratio, confidence_CC_THRESHOLD, Passband_High, Passband_Low);
            double delay = delay_cc_white_second_cc_ratio[0];

            confidence_CC = delay_cc_white_second_cc_ratio[1];
            speech_ratio = delay_cc_white_second_cc_ratio[5] * 100;

            // if there is no sound source at 0deg, regraded the cc at 0deg as white noise

#endif
            // no obvious sound detect
            if (no_obvious_sound_count > no_obvious_count_threshold)
            {
                // vis_tool.write_angles_to_txt(-1000, -1000);
                // cout << "&&&&&&&&&&&&&&&&&&&&&\n****************\n";
                if (obvious_sound_count == 0)
                {
                    SP_tool.theta_filtered = SP_tool.get_theta(delay);
                    theta_output = SP_tool.theta_filtered;
                    KF.x_hat(0) = SP_tool.theta_filtered;
                    KF.x_hat(1) = 0;
                }
                if (obvious_sound_count > obvious_strict_sequence)
                {
                    obvious_sound_count = 0;
                }
            }
            // update the deviation of measurment data
            R << R_init;

            // get the theta through delays

            theta_output = SP_tool.get_theta(delay);
            possibility = sqrt(KF.Possiblity_of_coherent_source(SP_tool.theta) * 1000);

            if ((possibility < single_measure_tolerance) || (delay == -20))
            {

                if (delay_cc_white_second_cc_ratio[3] != -20)
                {
                    delay = delay_cc_white_second_cc_ratio[3];
                    theta_output = SP_tool.get_theta(delay);
            possibility = sqrt(KF.Possiblity_of_coherent_source(SP_tool.theta) * 1000);

                    if (possibility < single_measure_tolerance)
                    {
                        delay = -20;
                        KF.R << 1e10;
                        SP_tool.theta = -120;
                        KF.x_hat(1) *= speed_attenuation_ratio;
                        obvious_sound_count = 0;
                    }
                }
                else
                {
                    delay = -20;
                    KF.R << 1e10;
                    SP_tool.theta = -120;
                    KF.x_hat(1) *= speed_attenuation_ratio;

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
                KF.x_hat(1) *= speed_attenuation_ratio;
                obvious_sound_count = 0;
            }
            else if ((speech_ratio < speech_ratio_threshold) && (obvious_sound_count > obvious_strict_sequence - 1))
            {
                delay = -20;
                KF.R << 1e10;
                SP_tool.theta = -120;
                KF.x_hat(1) *= speed_attenuation_ratio;

                obvious_sound_count = 0;
            }
#endif

            if ((no_obvious_sound_count > no_obvious_count_threshold) && (obvious_sound_count < obvious_strict_sequence))
            {
                delay = -20;
                KF.R << 1e10;
                SP_tool.theta = -120;
                KF.x_hat(1) *= speed_attenuation_ratio;

                no_obvious_sound_count = no_obvious_count_threshold + 2;
            }
            if (obvious_sound_count > obvious_strict_sequence - 1)
            {
                no_obvious_sound_count = 0;
            }

            t += dt * traking_speed_amplification;
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

            //@2nd STEP choosen data feeds kalman model

            SP_tool.theta_filtered = (KF.state())[0];

            // timer end
            auto end = system_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            double duration_time = double(duration.count()) * microseconds::period::num / microseconds::period::den;
            //@ 3rd STEP update kalman due to time interval

            A(0, 1) = duration_time;
            dt = duration_time;
            if (no_obvious_sound_count > no_obvious_count_threshold)
            {
                vis_tool.write_angles_to_txt(-900, SP_tool.theta_filtered);
            }
            else
            {
                vis_tool.write_angles_to_txt(SP_tool.theta, SP_tool.theta_filtered);
            }

            // if (no_obvious_sound_count < no_obvious_count_threshold)
            // {
// for accuracy calculation
#ifndef ON_RKCHIP_FLAG
            SP_tool.show_accuracy(reference_angle);
#endif
            // cout <<int(SP_tool.theta_filtered)<<endl;
            // visualize
            if ((i + k) % 200 == 1)
            {
                rk_config();
            }

            cout << "##" << left << setw(6) << i + k;
            if ((no_obvious_sound_count > no_obvious_count_threshold))
            {
                cout << " (----)";
            }
            else
            {
                cout << " (++++)";
            }

            cout << left << setw(7) << "  Theta: " << left << setw(8) << fixed << setprecision(1) << SP_tool.theta_filtered << "        ||   ";

            if (volume1 > volume_threshold)
            {
                cout << "(+)";
            }
            else
            {
                cout << "(-)";
            }
            cout << left << setw(7) << fixed << setprecision(2) << volume1;

            if (confidence_CC > confidence_CC_THRESHOLD)
            {
                cout << "(+)";
            }
            else
            {
                cout << "(-)";
            }
            cout << left << setw(7) << fixed << setprecision(2) << confidence_CC;

            if (speech_ratio > speech_ratio_threshold)
            {
                cout << "(+)";
            }
            else
            {
                cout << "(-)";
            }
            cout << left << setw(4) << fixed << setprecision(0) << speech_ratio;

            if (possibility > single_measure_tolerance)
            {
                cout << "(+)";
            }
            else
            {
                cout << "(-)";
            }
            cout << left << setw(7) << fixed << setprecision(2) << possibility;
            cout << "||" << endl;

            // print
            // cout << "##" << left << setw(7) << i + k << left << setw(7) << "theta@@: " << left << setw(9) << fixed << setprecision(2) << theta_output << left << setw(7) << "filtered: " << left << setw(8) << fixed << setprecision(2) << SP_tool.theta_filtered << left << setw(5) << "Volume:" << left << setw(4) << volume1 << left << setw(13) << "   sample_fre: " << left << setw(5) << fixed << setprecision(0) << 1 / duration_time << "Hz";
            // cout << left << setw(7) << "   CC: " << left << setw(9) << fixed << setprecision(2) << confidence_CC << left << setw(15) << "Speech_Ratio: " << left << setw(7) << fixed << setprecision(2) << speech_ratio << left << setw(10) << "coherent_p: " << possibility << endl;

            // }
        }
    }
}

void rk_config()
{
    // cout << "start configing" << endl;
#ifdef ON_RKCHIP_FLAG
    ifstream myfile("./RK_config");
#else
    ifstream myfile("../RK_config");
#endif

    string line;
    int num_line = 0;
    if (myfile.is_open())
    {

        while (getline(myfile, line))
        {
            num_line++;
            // cout << line << '\n';
            if (num_line % 3 == 2)
            {
                switch (num_line / 3 + 1)
                {
                case 1:
                    confidence_CC_THRESHOLD = stod(line);
                    break;
                case 2:
                    no_obvious_count_threshold = stoi(line);
                    break;
                case 3:
                    speech_ratio_threshold = stod(line);
                    break;
                case 4:
                    single_measure_tolerance = stod(line);
                    break;
                case 5:
                    obvious_strict_sequence = stoi(line);
                    break;
                case 6:
                    speed_attenuation_ratio = stod(line);
                    break;
                case 7:
                    traking_speed_amplification = stod(line);
                    break;
                case 8:
                    volume_threshold = stod(line);
                    break;
                case 9:
                    Passband_Low = stod(line);
                    break;
                case 10:
                    Passband_High = stod(line);
                    break;
                case 11:
                    reference_angle = stod(line);
                    break;
                case 12:
                    train_path = line;
                    break;

                default:
                    break;
                }
            }
        }

        myfile.close();
        // cout<<"Config successfully!"<<endl;
    }
}
