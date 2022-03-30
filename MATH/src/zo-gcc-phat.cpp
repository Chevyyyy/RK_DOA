#include "zo-gcc-phat.hpp"
#include "AudioFFT.h"
#include <complex>
#include <algorithm>
#include <iostream>
#include <cfloat>
#include <string.h>

namespace zo
{
    class GccPhatImpl : public GccPhat
    {
    public:
        int sample_cnt;
        audiofft::AudioFFT fft;

        void init(const int cnt) override
        {

            fft.init(cnt);
            sample_cnt = cnt;
        }

        void terminate() override
        {
            // _forward_FFT->terminate();
            // delete _forward_FFT;
            // _inverse_FFT->terminate();
            // delete _inverse_FFT;
        }

        void execute(const std::vector<float> &siga, const std::vector<float> &sigb, int margin, double *arg_max)
        {

            double volumeA = 0;
            double volumeB = 0;

            for (int j = 0; j < sample_cnt; j++)
            {
                volumeA += siga[j] * siga[j] / sample_cnt;
                volumeB += sigb[j] * sigb[j] / sample_cnt;
            }

            std::complex<float> I = std::complex<float>(0, 1);
            std::vector<std::complex<float>> siga_fft(sample_cnt);
            std::vector<float> reA(sample_cnt / 2);
            std::vector<float> imA(sample_cnt / 2);
            fft.fft(siga.data(), reA.data(), imA.data());
            double sum = 0;
            for (int i = 0; i < sample_cnt / 2; i++)
            {

                siga_fft[i] = reA[i] + I * imA[i];
                siga_fft[sample_cnt - i - 1] = reA[i] + I * imA[i];

                if ((i > 1) && (i < 12))
                {
                    sum += (reA[i] * reA[i] + imA[i] * imA[i]) / RANGE;
                }
            }
            sum = log10(sum);

            std::vector<std::complex<float>> sigb_fft(sample_cnt);
            std::vector<float> reB(sample_cnt / 2);
            std::vector<float> imB(sample_cnt / 2);
            fft.fft(sigb.data(), reB.data(), imB.data());

            for (int i = 0; i < sample_cnt / 2; i++)
            {

                sigb_fft[i] = reB[i] + I * imB[i];
                sigb_fft[sample_cnt - i - 1] = reB[i] + I * imB[i];
            }
            // R = SIG * REFSIG_CONJ
            std::vector<std::complex<float>> R;
            R.resize(siga_fft.size());
            for (int i = 0; i < siga_fft.size(); i++)
            {
                std::complex<float> v = sigb_fft[i] * std::conj(siga_fft[i]);
                std::complex<float> v1 = sigb_fft[i] * std::conj(sigb_fft[i]);
                v = v / (std::abs(v) + FLT_MIN); // phat
                // v = v * std::abs(v1) / std::abs((std::abs(v) + FLT_MIN) * (1 - std::abs(v1)));//ML
                // v = v / (std::abs(v1) + FLT_MIN);//Roth

                R[i] = v; // cc
            }

            // Inverse

            std::vector<float> reR(sample_cnt);
            std::vector<float> imR(sample_cnt);
            cross_correlation.resize(sample_cnt);
            for (int i = 0; i < R.size(); i++)
            {
                reR[i] = R[i].real();
                imR[i] = R[i].imag();
            }

            // calculate_inverse_fft(cross_correlation, R, n);
            fft.ifft(cross_correlation.data(), reR.data(), imR.data());

            /*
             * Shift the values in xcorr[] so that the 0th lag is at the center of
             * the output array.
             * [Note: the index of the center value in the output will be: ceil(_N/2) ]
             */
            std::vector<float> shifted;
            shift<float>(shifted, cross_correlation);

            // First, make sure the margin is within the bounds of the computed lags
            int n = cross_correlation.size();
            double center_i = ceil(n / 2.0);
            double newmargin = margin;
            if (((int)(center_i - newmargin)) < 0)
            {
                newmargin = center_i;
            }
            if ((center_i + newmargin) >= n)
            {
                newmargin = (n - 1) - center_i;
            }

            /* Compute the begin index and length of the lags_loc[] array */
            double start_i = center_i - newmargin;
            double len = 2 * newmargin + 1;

            // calculate argmax
            int max_index = std::distance(shifted.begin() + start_i, std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len));

            arg_max[0] = max_index - newmargin;
            arg_max[1] = *std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len) / RANGE;
            arg_max[2] = sum;
            volume_index = arg_max[1];
        }
        void PHAT_SRP_4mic(Wave1234 *wave1234, int margin, double *arg_max, double white_cc)
        {

            float mean1 = std::accumulate(wave1234->ch1.begin(), wave1234->ch1.end(), 0) / sample_cnt;
            float mean2 = std::accumulate(wave1234->ch2.begin(), wave1234->ch2.end(), 0) / sample_cnt;
            float mean3 = std::accumulate(wave1234->ch3.begin(), wave1234->ch3.end(), 0) / sample_cnt;
            float mean4 = std::accumulate(wave1234->ch4.begin(), wave1234->ch4.end(), 0) / sample_cnt;
            for (int i = 0; i < sample_cnt; i++)
            {
                wave1234->ch1[i] = wave1234->ch1[i] - mean1;
                wave1234->ch2[i] = wave1234->ch2[i] - mean2;
                wave1234->ch3[i] = wave1234->ch3[i] - mean3;
                wave1234->ch4[i] = wave1234->ch4[i] - mean4;
            }
            std::vector<float> cross_correlation_sum(sample_cnt);

            execute(wave1234->ch1, wave1234->ch2, 5, arg_max);
            std::vector<float> cross_correlation_1(cross_correlation);
            execute(wave1234->ch2, wave1234->ch3, 5, arg_max);
            std::vector<float> cross_correlation_2(cross_correlation);
            execute(wave1234->ch3, wave1234->ch4, 5, arg_max);
            std::vector<float> cross_correlation_3(cross_correlation);

            for (int i = 0; i < sample_cnt; i++)
            {
                // cross_correlation_3[i]
                cross_correlation_sum[i] = (cross_correlation_1[i] + cross_correlation_2[i] + cross_correlation_3[i]) / 3.0;
            }

            execute(wave1234->ch1, wave1234->ch3, 10, arg_max);
            std::vector<float> cross_correlation_4(cross_correlation);
            execute(wave1234->ch2, wave1234->ch4, 10, arg_max);
            std::vector<float> cross_correlation_5(cross_correlation);
            execute(wave1234->ch1, wave1234->ch4, 15, arg_max);
            std::vector<float> cross_correlation_6(cross_correlation);

            for (int i = 0; i < sample_cnt / 2; i++)
            {
                // cross_correlation_3[i]
                cross_correlation_sum[i] = (cross_correlation_1[(i +2)/ 3] + cross_correlation_2[(i +2) / 3] + cross_correlation_3[(i +2)/ 3] + cross_correlation_4[(i +1) / 1.5] + cross_correlation_5[(i+1) / 1.5] + cross_correlation_6[i]) / 6.0;
                cross_correlation_sum[sample_cnt-i-1] = (cross_correlation_1[sample_cnt - i / 3 - 1] + cross_correlation_2[sample_cnt - i / 3 - 1] + cross_correlation_3[sample_cnt - i / 3 - 1] + cross_correlation_4[sample_cnt - i / 1.5 - 1] + cross_correlation_5[sample_cnt - i / 1.5 - 1] + cross_correlation_6[sample_cnt - i - 1]) / 6.0;
            }

            double white_0_cc = cross_correlation_sum[0];
            cross_correlation_sum[0] = cross_correlation_sum[0] - white_cc;
            // cross_correlation_sum[1] = cross_correlation_sum[1] - white_cc / 2;
            // cross_correlation_sum[sample_cnt - 1] = cross_correlation_sum[sample_cnt - 1] - white_cc / 2;
            /*
             * Shift the values in xcorr[] so that the 0th lag is at the center of
             * the output array.
             * [Note: the index of the center value in the output will be: ceil(_N/2) ]
             */
            std::vector<float> shifted;
            shift<float>(shifted, cross_correlation_sum);

            // First, make sure the margin is within the bounds of the computed lags
            int n = cross_correlation_sum.size();
            double center_i = ceil(n / 2.0);
            double newmargin = 25;
            if (((int)(center_i - newmargin)) < 0)
            {
                newmargin = center_i;
            }
            if ((center_i + newmargin) >= n)
            {
                newmargin = (n - 1) - center_i;
            }

            /* Compute the begin index and length of the lags_loc[] array */
            double start_i = center_i - newmargin;
            double len = 2 * newmargin + 1;

            // calculate argmax
            int max_index = std::distance(shifted.begin() + start_i, std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len));

            arg_max[0] = (max_index - newmargin) / 3.0;
            arg_max[1] = *std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len);
            arg_max[2] = white_0_cc;

            // the second source
            shifted[sample_cnt / 2 - newmargin + max_index] = -10;

            max_index = std::distance(shifted.begin() + start_i, std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len));

            arg_max[3] = (max_index - newmargin) / 3.0;
            arg_max[4] = *std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len);

            if (arg_max[1] < confidence_CC_THRESHOLD)
            {
                arg_max[0] = -20;
            }
            if (arg_max[4] < confidence_CC_THRESHOLD)
            {
                arg_max[3] = -20;
            }

            // omit fake 0deg
            if ((abs(arg_max[0]) < 0.0001) && ((cross_correlation_sum[1] < 0) && (cross_correlation_sum[sample_cnt - 1] < 0)))
            {
                arg_max[0] = -20;
            }
            if ((abs(arg_max[3]) < 0.0001) && ((cross_correlation_sum[1] < 0) && (cross_correlation_sum[sample_cnt - 1] < 0)))
            {
                arg_max[3] = -20;
            }
        }
        void PHAT_SRP_3mic(Wave1234 *wave1234, int margin, double *arg_max, double white_cc)
        {
            float mean1 = std::accumulate(wave1234->ch1.begin(), wave1234->ch1.end(), 0) / sample_cnt;
            float mean2 = std::accumulate(wave1234->ch2.begin(), wave1234->ch2.end(), 0) / sample_cnt;
            float mean3 = std::accumulate(wave1234->ch3.begin(), wave1234->ch3.end(), 0) / sample_cnt;
            float mean4 = std::accumulate(wave1234->ch4.begin(), wave1234->ch4.end(), 0) / sample_cnt;
            for (int i = 0; i < sample_cnt; i++)
            {
                wave1234->ch1[i] = wave1234->ch1[i] - mean1;
                wave1234->ch2[i] = wave1234->ch2[i] - mean2;
                wave1234->ch3[i] = wave1234->ch3[i] - mean3;
                wave1234->ch4[i] = wave1234->ch4[i] - mean4;
            }
            std::vector<float> cross_correlation_sum(sample_cnt);

            execute(wave1234->ch1, wave1234->ch2, 5, arg_max);
            std::vector<float> cross_correlation_1(cross_correlation);
            execute(wave1234->ch2, wave1234->ch3, 5, arg_max);
            std::vector<float> cross_correlation_2(cross_correlation);
            execute(wave1234->ch1, wave1234->ch3, 10, arg_max);
            std::vector<float> cross_correlation_3(cross_correlation);
            for (int i = 0; i < sample_cnt; i++)
            {
                // cross_correlation_3[i]
                cross_correlation_sum[i] = (cross_correlation_1[i / 2] + cross_correlation_2[i / 2] + cross_correlation_3[i]) / 3;
            }
            arg_max[2] = cross_correlation_sum[0];

            cross_correlation_sum[0] = cross_correlation_sum[0] - white_cc;
            cross_correlation_sum[1] = cross_correlation_sum[1] - white_cc / 4;
            cross_correlation_sum[sample_cnt - 1] = cross_correlation_sum[sample_cnt - 1] - white_cc / 4;
            /*
             * Shift the values in xcorr[] so that the 0th lag is at the center of
             * the output array.
             * [Note: the index of the center value in the output will be: ceil(_N/2) ]
             */
            std::vector<float> shifted;
            shift<float>(shifted, cross_correlation_sum);

            // First, make sure the margin is within the bounds of the computed lags
            int n = cross_correlation_sum.size();
            double center_i = ceil(n / 2.0);
            double newmargin = 25;
            if (((int)(center_i - newmargin)) < 0)
            {
                newmargin = center_i;
            }
            if ((center_i + newmargin) >= n)
            {
                newmargin = (n - 1) - center_i;
            }

            /* Compute the begin index and length of the lags_loc[] array */
            double start_i = center_i - newmargin;
            double len = 2 * newmargin + 1;

            // calculate argmax
            int max_index = std::distance(shifted.begin() + start_i, std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len));

            arg_max[0] = (max_index - newmargin) / 2.0;
            arg_max[1] = *std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len);

            if (arg_max[1] < confidence_CC_THRESHOLD)
            {
                arg_max[0] = -20;
            }
        }
        void PHAT_SRP_2mic(Wave1234 *wave1234, int margin, double *arg_max, double white_cc)
        {
            float mean1 = std::accumulate(wave1234->ch1.begin(), wave1234->ch1.end(), 0) / sample_cnt;
            float mean2 = std::accumulate(wave1234->ch2.begin(), wave1234->ch2.end(), 0) / sample_cnt;
            float mean3 = std::accumulate(wave1234->ch3.begin(), wave1234->ch3.end(), 0) / sample_cnt;
            float mean4 = std::accumulate(wave1234->ch4.begin(), wave1234->ch4.end(), 0) / sample_cnt;
            for (int i = 0; i < sample_cnt; i++)
            {
                wave1234->ch1[i] = wave1234->ch1[i] - mean1;
                wave1234->ch2[i] = wave1234->ch2[i] - mean2;
                wave1234->ch3[i] = wave1234->ch3[i] - mean3;
                wave1234->ch4[i] = wave1234->ch4[i] - mean4;
            }

            execute(wave1234->ch1, wave1234->ch2, 5, arg_max);
            std::vector<float> cross_correlation_sum(cross_correlation);
            double white_0_cc = cross_correlation_sum[0];
            cross_correlation_sum[0] = cross_correlation_sum[0] - white_cc;
            // cross_correlation_sum[1] = cross_correlation_sum[1] - white_cc / 4;
            // cross_correlation_sum[sample_cnt - 1] = cross_correlation_sum[sample_cnt - 1] - white_cc / 4;
            /*
             * Shift the values in xcorr[] so that the 0th lag is at the center of
             * the output array.
             * [Note: the index of the center value in the output will be: ceil(_N/2) ]
             */
            std::vector<float> shifted;
            shift<float>(shifted, cross_correlation_sum);

            // First, make sure the margin is within the bounds of the computed lags
            int n = cross_correlation_sum.size();
            double center_i = ceil(n / 2.0);
            double newmargin = 25;
            if (((int)(center_i - newmargin)) < 0)
            {
                newmargin = center_i;
            }
            if ((center_i + newmargin) >= n)
            {
                newmargin = (n - 1) - center_i;
            }

            /* Compute the begin index and length of the lags_loc[] array */
            double start_i = center_i - newmargin;
            double len = 2 * newmargin + 1;

            // calculate argmax
            int max_index = std::distance(shifted.begin() + start_i, std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len));

            arg_max[0] = (max_index - newmargin);
            arg_max[1] = *std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len);
            arg_max[2] = white_0_cc;

            if (arg_max[1] < confidence_CC_THRESHOLD)
            {
                arg_max[0] = -20;
            }
        }
        void PHAT_SRP_2mic_times_4(Wave1234 *wave1234, int margin, double *arg_max, double white_cc)
        {

            float mean1 = std::accumulate(wave1234->ch1.begin(), wave1234->ch1.end(), 0) / sample_cnt;
            float mean2 = std::accumulate(wave1234->ch2.begin(), wave1234->ch2.end(), 0) / sample_cnt;

            for (int i = 0; i < sample_cnt; i++)
            {
                wave1234->ch1[i] = wave1234->ch1[i] - mean1;
                wave1234->ch2[i] = wave1234->ch2[i] - mean2;
            }
            sample_cnt = RANGE / 4;
            fft.init(sample_cnt);
            std::vector<float> cross_correlation_sum(sample_cnt);
            std::vector<float> first_seg_wav1(wave1234->ch1.begin(), wave1234->ch1.begin() + sample_cnt);
            std::vector<float> second_seg_wav1(wave1234->ch1.begin() + sample_cnt, wave1234->ch1.begin() + sample_cnt * 2);
            std::vector<float> third_seg_wav1(wave1234->ch1.begin() + sample_cnt * 2, wave1234->ch1.begin() + sample_cnt * 3);
            std::vector<float> forth_seg_wav1(wave1234->ch1.begin() + sample_cnt * 3, wave1234->ch1.begin() + sample_cnt * 4);

            std::vector<float> first_seg_wav2(wave1234->ch2.begin(), wave1234->ch2.begin() + sample_cnt);
            std::vector<float> second_seg_wav2(wave1234->ch2.begin() + sample_cnt, wave1234->ch2.begin() + sample_cnt * 2);
            std::vector<float> third_seg_wav2(wave1234->ch2.begin() + sample_cnt * 2, wave1234->ch2.begin() + sample_cnt * 3);
            std::vector<float> forth_seg_wav2(wave1234->ch2.begin() + sample_cnt * 3, wave1234->ch2.begin() + sample_cnt * 4);

            execute(first_seg_wav1, first_seg_wav2, 5, arg_max);
            std::vector<float> cross_correlation_1(cross_correlation);
            execute(second_seg_wav1, second_seg_wav2, 5, arg_max);
            std::vector<float> cross_correlation_2(cross_correlation);
            execute(third_seg_wav1, third_seg_wav2, 5, arg_max);
            std::vector<float> cross_correlation_3(cross_correlation);
            execute(forth_seg_wav1, forth_seg_wav2, 5, arg_max);
            std::vector<float> cross_correlation_4(cross_correlation);

            for (int i = 0; i < sample_cnt; i++)
            {
                // cross_correlation_3[i]
                cross_correlation_sum[i] = (cross_correlation_1[i] + cross_correlation_2[i] + cross_correlation_3[i] + cross_correlation_4[i]) / 4;
            }
            cross_correlation_sum[0] = cross_correlation_sum[0] - white_cc;
            /*
             * Shift the values in xcorr[] so that the 0th lag is at the center of
             * the output array.
             * [Note: the index of the center value in the output will be: ceil(_N/2) ]
             */
            std::vector<float> shifted;
            shift<float>(shifted, cross_correlation_sum);

            // First, make sure the margin is within the bounds of the computed lags
            int n = cross_correlation_sum.size();
            double center_i = ceil(n / 2.0);
            double newmargin = 25;
            if (((int)(center_i - newmargin)) < 0)
            {
                newmargin = center_i;
            }
            if ((center_i + newmargin) >= n)
            {
                newmargin = (n - 1) - center_i;
            }

            /* Compute the begin index and length of the lags_loc[] array */
            double start_i = center_i - newmargin;
            double len = 2 * newmargin + 1;

            // calculate argmax
            int max_index = std::distance(shifted.begin() + start_i, std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len));

            arg_max[0] = (max_index - newmargin);
            arg_max[1] = *std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len);
            arg_max[2] = volume_index;

            if (arg_max[1] < confidence_CC_THRESHOLD)
            {
                arg_max[0] = -20;
            }
        }

    protected:
        /**
         * Shift the output of an FFT.
         *
         * The index of the mid-point in the output will be located at: ceil(_N/2)
         * @ingroup GCC
         */
        template <typename SCALAR = float>
        void shift(std::vector<SCALAR> &out, const std::vector<SCALAR> &in)
        {
            const size_t N = in.size();

            // mid-point of out[] will be located at index ceil(N/2)
            const size_t xx = (size_t)std::floor((SCALAR)N / 2.0);

            auto in_xx_iter = in.begin() + xx;

            out.clear();

            // Copy last half of in[] to first half of out[]
            std::copy(in_xx_iter, in.end(), std::back_inserter(out));

            // Copy first half of in[] to end of out[]
            std::copy(in.begin(), in_xx_iter, std::back_inserter(out));

            // Copy last half of in[] to first half of out[]
            // memcpy(&out[0],&in[xx],sizeof(float)*(N-xx));

            // Copy first half of in[] to end of out[]
            // memcpy(&out[N-xx],&in[0],sizeof(float)*xx);
        }

    protected:
        // FFT_forward* _forward_FFT = nullptr;
        // FFT_inverse* _inverse_FFT = nullptr;
    };

    GccPhat *GccPhat::create()
    {
        return new GccPhatImpl();
    }
} // namespace zo