#define LPF_CUTOFF 100
#define mic_distance 0.035
#define Vs 342 // speed of sound
#define RANGE 512
#define SAMPLE_RATE 44100
#define SHOW_RAW_DATA 0
#define DELAY_MAX mic_distance / Vs *SAMPLE_RATE
#define DELTA 1 / 44100.0
#define PI 3.1415926
#define Degree_To_Rad(degree) degree *PI / 180

// #define ON_RKCHIP_FLAG
// #define VISUAL_QT_FLAG

// dataset
//  #define refernce_angle 90
//  #define train_path "../sound_data/labelled_dataset/30s_90deg_cafe.wav"

// #define refernce_angle 45
// #define train_path "../sound_data/labelled_dataset/30s_45deg_cafe.wav"

// #define refernce_angle 64
// #define train_path "../sound_data/labelled_dataset/30s_64deg_cafe.wav"

#define refernce_angle 25
#define train_path "../sound_data/labelled_dataset/10s_25deg_speech.wav"

// #define refernce_angle -30
// #define train_path "../sound_data/labelled_dataset/10s_-30deg_speech.wav"

#define confidence_CC_THRESHOLD 0.1
#define no_obvious_count_threshold 20
#define single_measure_tolerance 10
#define obvious_strict_sequence 2

// MATH algorithm
#define PHAT_SPR
// #define GCC_PHAT
