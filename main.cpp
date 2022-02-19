#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "cc.hpp"
#include "cmath"
#include "unistd.h"
#include "iomanip"

int theta = -10;
int theta_last = -10;
double average_delay = 0;
double average_cc = 0;
int flag = 0;
Wav wav;
Wave1234 wave1234;
    
int main()
{
    FILE *fp = NULL;

    double theta_array1[average_filter_length] = {0};
    double theta_array2[average_filter_length] = {0};

    for (int i = 0; i < 1000000000; i++)
    {

        record();
        fp = fopen("data/test.wav", "rb");
        if (!fp)
        {
            printf("can't open audio file\n");
            // exit(1);
        }
        fread(&wav, 1, sizeof(wav), fp);
        // cout << "#########################" << endl;
        // printf("blockSize \t%d\n", wav.data.Subchunk2Size);
        fclose(fp);
        wave_to_chs(0, 0);

        int vol = CC(wave1234, wave1234, range);
        average_cc = (delay1234.delay12.cc + delay1234.delay23.cc + delay1234.delay34.cc) / 6;
        // average_delay = ((delay1234.delay12.cc / average_cc) * (delay1234.delay12.cc / average_cc) * delay1234.delay12.delay + (delay1234.delay23.cc / average_cc) * (delay1234.delay23.cc / average_cc) * delay1234.delay23.delay + (delay1234.delay34.cc / average_cc) * (delay1234.delay34.cc / average_cc) * delay1234.delay34.delay + 2 * (delay1234.delay13.cc / average_cc) * (delay1234.delay13.cc / average_cc) * delay1234.delay13.delay + 2 * (delay1234.delay24.cc / average_cc) * (delay1234.delay24.cc / average_cc) * delay1234.delay24.delay + 3 * (delay1234.delay14.cc / average_cc) * (delay1234.delay14.cc / average_cc) * delay1234.delay14.delay) / 10;
        average_delay = ((delay1234.delay12.cc / average_cc) * delay1234.delay12.delay + (delay1234.delay23.cc / average_cc) * delay1234.delay23.delay + (delay1234.delay34.cc / average_cc) * delay1234.delay34.delay) / 3;
        // average_delay=delay1234.delay12.delay ;

        if (average_delay > 4.5)
        {
            average_delay = 4.5;
        }
        if (average_delay < -4.5)
        {
            average_delay = -4.5;
        }
        if (average_cc == 0)
        {
            average_delay = 0;
        }

        theta = 180 * asin((average_delay * Vs) / (44100 * mic_distance)) / 3.1415926;
        if (vol < 5000)
        {
            theta = -1000;
            flag = 1;
        }
        else
        {
            flag = 0;
        }
        // else
        // {
        cout << "vol:  " << vol << "\n";
        //     theta_last = theta;
        // }
        if (flag == 0)
        {
            for (int k = average_filter_length - 1; k > 0; k--)
            {

                theta_array1[k] = theta_array1[k - 1];
                // double tmp=theta_array1[k];
            }
            theta_array1[0] = theta;
            double sum = 0;
            for (int k = 0; k < average_filter_length; k++)
            {
                sum += theta_array1[k];
            }
            theta = sum / average_filter_length;
        }
        // for (int k = 0; k < average_filter_length - 1; k++)
        // {
        //     theta_array2[k + 1] = theta_array2[k];
        // }
        // theta_array2[0] = theta;
        //  sum = 0;
        // for (int k = 0; k < average_filter_length; k++)
        // {
        //     sum += theta_array2[k];
        // }
        // theta = sum / average_filter_length;

        visualize(theta);

        // usleep(1000);

        // if ((theta < 91) && (theta > -91))
        // {
        //     visualize(theta);
        // }
        //     if(average_cc>0.3)
        //     {
        //    data_print();
        // //    cout<<"/n"<<average_cc<<"************";
        //    cout<<"/n"<<vol<<"************";
        //     }

        // usleep(10000);
    }
}

void visualize(double theta)
{

    int r = 20;
    int o = 40;
    int locate = r * sin(3.1415926 * ((90 - abs(theta)) / 180)) - o;
    if (locate == (r - o))
    {
        cout << "\n                             -------OOO-----\n";
    }
    else
    {
        cout << "\n                             ---------------\n";
    }

    for (int Y = r - o - 1; Y > -1 * o; Y--)
    {

        int x_axis = sqrt(abs(r * r - (Y + o) * (Y + o)));
        if ((Y != locate) || (theta == -1000))
        {
            cout << setw(o - x_axis) << "------" << setw(2 * x_axis) << "------";
        }
        else if ((theta > 0) && (theta < 90))
        {
            cout << setw(o - x_axis) << "------" << setw(2 * x_axis) << "-OOO-";
        }
        else if ((theta < 0) && (theta > -90))
        {
            cout << setw(o - x_axis) << "-OOO-" << setw(2 * x_axis) << "------";
        }
        else
        {
            cout << setw(o - x_axis) << "------" << setw(2 * x_axis) << "------";
        }

        cout << "\n";
    }
    if (theta == -1000)
    {
        cout << "theta:  "
             << "no sound!"
             << "\n";
    }
    else
    {
        cout << "theta:  " << theta << "\n";
    }
}

void delay_print()
{
    cout << "\n\n\n\n\n";

    cout << "####################################\n";
    cout << "####################################\n";
    cout << "max_cc12:" << delay1234.delay12.cc << "\n";
    cout << "true_delay12:" << delay1234.delay12.delay << "\n";
    cout << "####################################\n";
    cout << "max_cc13:" << delay1234.delay13.cc << "\n";
    cout << "true_delay13:" << delay1234.delay13.delay << "\n";
    cout << "####################################\n";
    cout << "max_cc14:" << delay1234.delay14.cc << "\n";
    cout << "true_delay14:" << delay1234.delay14.delay << "\n";
    cout << "####################################\n";
    cout << "true_delay23:" << delay1234.delay23.delay << "\n";
    cout << "max_cc23:" << delay1234.delay23.cc << "\n";
    cout << "####################################\n";
    cout << "max_cc24:" << delay1234.delay24.cc << "\n";
    cout << "true_delay24:" << delay1234.delay24.delay << "\n";
    cout << "####################################\n";
    cout << "max_cc34:" << delay1234.delay34.cc << "\n";
    cout << "true_delay34:" << delay1234.delay34.delay << "\n";
    cout << "####################################\n";
    cout << "####################################\n";
    cout << "####################################\n";

    cout << "theta:  " << theta << "\n";
}