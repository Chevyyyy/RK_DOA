#define LPF_CUTOFF 100
#define mic_distance 0.035
#define Vs 342 // speed of sound
#define RANGE 1024
#define SAMPLE_RATE 44100   
#define SHOW_RAW_DATA 0
#define DELAY_MAX mic_distance/Vs*SAMPLE_RATE
#define DELTA  1/44100.0
#define PI 3.1415926
#define Degree_To_Rad(degree) degree*PI/180




#define ON_RKCHIP_FLAG
// #define VISUAL_QT_FLAG
#define refernce_angle 30


#define VOLUME_THRESHOLD 0.0
#define train_path "../sound_data/labelled_dataset/10s_30deg_speech.wav"

#define confidence_CC_THRESHOLD 0.15
#define no_obvious_count_threshold 20

//MATH algorithm
#define PHAT_SPR
// #define GCC_PHAT

