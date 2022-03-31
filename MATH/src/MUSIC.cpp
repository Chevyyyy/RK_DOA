#include "MUSIC.hpp"

MUSIC::MUSIC(/* args */)
{
}

MUSIC::~MUSIC()
{
}

MUSIC::MUSIC(int mic_num1, int source_num1, int frame_range1, int mic_distance1)
{
    mic_num = mic_num1;
    source_num = source_num1;
    frame_range = frame_range1;
    mic_dd = mic_distance1;
}

void MUSIC::update(Wave1234 *ch4_wave_mic)
{
    double target_fre = 628;
    MatrixXf X(mic_num, frame_range);
    float mean1 = std::accumulate(ch4_wave_mic->ch1.begin(), ch4_wave_mic->ch1.end(), 0) / frame_range;
    float mean2 = std::accumulate(ch4_wave_mic->ch2.begin(), ch4_wave_mic->ch2.end(), 0) / frame_range;
    float mean3 = std::accumulate(ch4_wave_mic->ch3.begin(), ch4_wave_mic->ch3.end(), 0) / frame_range;
    float mean4 = std::accumulate(ch4_wave_mic->ch4.begin(), ch4_wave_mic->ch4.end(), 0) / frame_range;

    for (int i = 0; i < X.cols(); i++)
    {
        X(0, i) = ch4_wave_mic->ch1[i] - mean1;
        X(1, i) = ch4_wave_mic->ch2[i] - mean2;
        X(2, i) = ch4_wave_mic->ch3[i] - mean3;
        X(3, i) = ch4_wave_mic->ch4[i] - mean4;
    }
    MatrixXf Rxx(mic_num, mic_num);
    Rxx = (X * X.transpose()) / frame_range; // covairance of mic data
    EigenSolver<MatrixXf> eig(Rxx);

    // get eigen values and vectors
    VectorXf Eig_values(mic_num);
    VectorXf Eig_values_cp(mic_num);
    MatrixXf Eig_vector(mic_num, mic_num);
    MatrixXf Eig_vector_cp(mic_num, mic_num);

    Eig_values << eig.eigenvalues().real();
    Eig_values_cp << eig.eigenvalues().real();
    Eig_vector << eig.eigenvectors().real().transpose();
    Eig_vector_cp << eig.eigenvectors().real().transpose();


    std::sort(Eig_values.begin(), Eig_values.end());
    for (int i = 0; i < mic_num; i++)
    {
        int x = std::find(Eig_values_cp.begin(), Eig_values_cp.end(), Eig_values(i)) - Eig_values_cp.begin();
        for (int k = 0; k < mic_num; k++)
        {
            Eig_vector(i, k) = Eig_vector_cp(x, k);
        }
    }


    VectorXf Pmusic(180);
    for (int i = 0; i < 180; i++)
    {
        Vector4f mic_geo;
        for (int k = 0; k < mic_num; k++)
        {
            mic_geo(k) = mic_distance * k;
        }

        double rad = Degree_To_Rad(i-90);
        MatrixXcf alpha(1,mic_num);
        
        for (int j = 0; j < mic_num; j++)
        {
            alpha.real()(0,j) = cos(-2 * PI * target_fre * mic_geo(j) * sin(rad)/Vs);
            alpha.imag()(0,j) = sin(-2 * PI * target_fre * mic_geo(j) * sin(rad)/Vs);
        }
        MatrixXf En(mic_num - source_num, mic_num);
        En = Eig_vector.block(0, 0, mic_num - source_num, mic_num);
        VectorXcf res(1, 1);
        res << (alpha * En.transpose() * En * alpha.transpose());
        Pmusic(i) = 1/sqrt(res.real()(0, 0) * res.real()(0, 0) + res.imag()(0, 0) * res.imag()(0, 0));
        Pmusic(0) = 0;
    }
    int angle = 90-std::distance(Pmusic.begin(), std::max_element(Pmusic.begin(), Pmusic.end()));
    std::cout << "anglePP" << ": " << angle << std::endl;
}
