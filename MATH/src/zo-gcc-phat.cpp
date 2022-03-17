#include "zo-gcc-phat.hpp"
#include "AudioFFT.h"
#include <complex>
#include <algorithm>
#include <iostream>
#include <cfloat>
#include <string.h>

namespace zo {
    class GccPhatImpl : public GccPhat {
    public:
        int sample_cnt;
        audiofft::AudioFFT fft;
        void init(const int cnt) override
        {
            
            fft.init(cnt);
            sample_cnt = cnt;
        }

        void terminate() override {
            // _forward_FFT->terminate();
            // delete _forward_FFT;
            // _inverse_FFT->terminate();
            // delete _inverse_FFT;
        }

        int execute(const std::vector<float>& siga, const std::vector<float>& sigb, int margin)  {
            std::complex<float> I = std::complex<float>(0, 1);
            std::vector<std::complex<float>> siga_fft(sample_cnt);
            std::vector<float> reA(audiofft::AudioFFT::ComplexSize(sample_cnt)); 
            std::vector<float> imA(audiofft::AudioFFT::ComplexSize(sample_cnt)); 
            fft.fft(siga.data(),reA.data(),imA.data());
            for (int i = 0; i <sample_cnt; i++) {
                siga_fft[i] = reA[i] + I * imA[i];
            }

            std::vector<std::complex<float>> sigb_fft(sample_cnt);
            std::vector<float> reB(audiofft::AudioFFT::ComplexSize(sample_cnt)); 
            std::vector<float> imB(audiofft::AudioFFT::ComplexSize(sample_cnt)); 
            fft.fft(sigb.data(),reB.data(),imB.data());
            for (int i = 0; i < sample_cnt; i++) {
                sigb_fft[i] = reB[i] + I * imB[i];
            }
            // R = SIG * REFSIG_CONJ
            std::vector<std::complex<float>> R;
            R.resize(siga_fft.size());            
            for (int i = 0; i < siga_fft.size(); i++) {
                std::complex<float> v = sigb_fft[i] * std::conj(siga_fft[i]);
                v = v / (std::abs(v) + FLT_MIN);
                R[i] = v;
            }


            // Inverse
            std::vector<float> cross_correlation(sample_cnt);
            std::vector<float> reR(sample_cnt); 
            std::vector<float> imR(sample_cnt); 
            for (int i = 0; i < R.size(); i++) {
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
            unsigned center_i = ceil(n / 2.0);
            unsigned newmargin=margin;
            if (((int)(center_i - newmargin)) < 0) {
                newmargin = center_i;
            }
            if ((center_i + newmargin) >= n) {
                newmargin = (n-1) - center_i;
            }

            /* Compute the begin index and length of the lags_loc[] array */
            unsigned start_i = center_i-newmargin;
            unsigned len = 2*newmargin+1;

            // calculate argmax
            int max_index = std::distance(shifted.begin() + start_i, std::max_element(shifted.begin() + start_i, shifted.begin() + start_i + len));
            int arg_max = max_index - newmargin;

            return (float)arg_max;
        }

    protected:

        /**
         * Shift the output of an FFT.
         *
         * The index of the mid-point in the output will be located at: ceil(_N/2)
         * @ingroup GCC
         */
        template<typename SCALAR=float>
        void shift(std::vector<SCALAR>& out, const std::vector<SCALAR>& in) {
            const size_t N = in.size();
            
            // mid-point of out[] will be located at index ceil(N/2) 
            const size_t xx = (size_t) std::floor((SCALAR) N/2.0);

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

    GccPhat* GccPhat::create() {
        return new GccPhatImpl();
    }
} // namespace zo