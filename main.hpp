#define LPF_CUTOFF 100
#define mic_distance 0.035
#define Vs 340 // speed of sound
#define RANGE 1024*4
#define SAMPLE_RATE 44100   
#define SHOW_RAW_DATA 0
#define DELAY_MAX mic_distance/Vs*SAMPLE_RATE
#define DELTA  1/44100.0
#define PI 3.1415926
#define Degree_To_Rad(degree) degree*PI/180




// #define ON_RKCHIP_FLAG
// #define VISUAL_QT_FLAG


#define VOLUME_THRESHOLD 0.0
#define train_path "../sound_data/labelled_dataset/30s_20deg_speech.wav"

#define white_noise_CC 0.8
#define confidence_CC_THRESHOLD 0.15    

//MATH algorithm
#define PHAT_SPR
// #define GCC_PHAT

