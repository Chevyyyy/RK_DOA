//@brief: basic constant
#define LPF_CUTOFF 100
#define mic_distance 0.0345
#define Vs 342 // speed of sound
#define RANGE 512
#define SAMPLE_RATE 44100
#define SHOW_RAW_DATA 0
#define DELAY_MAX mic_distance / Vs *SAMPLE_RATE
#define DELTA 1 / 44100.0
#define PI 3.1415926
#define Degree_To_Rad(degree) degree *PI / 180

//@brief: mode
#define Track_speech_flag
// #define ON_RKCHIP_FLAG




//@brief: MATH algorithm & hyperparameters
#define PHAT_SPR
#define confidence_CC_THRESHOLD 0.095
#define no_obvious_count_threshold 30
#define speech_ratio_threshold 70
#define single_measure_tolerance 1
#define obvious_strict_sequence 2
#define speed_attenuation_ratio 1



//@brief: dataset (testing)
 #define refernce_angle 90
 #define train_path "../sound_data/labelled_dataset/30s_90deg_cafe.wav"

// #define refernce_angle 45
// #define train_path "../sound_data/labelled_dataset/30s_45deg_cafe.wav"

// #define refernce_angle 64
// #define train_path "../sound_data/labelled_dataset/30s_64deg_cafe.wav"

// #define refernce_angle 25
// #define train_path "../sound_data/labelled_dataset/10s_25deg_speech.wav"

// #define refernce_angle -30
// #define train_path "../sound_data/labelled_dataset/10s_-30deg_speech.wav"

// #define refernce_angle -30
// #define train_path "../sound_data/labelled_dataset/10s_30deg_speech_0deg_music.wav"

//@brief: parameters (testing)
#define Usleep_time 48000
// #define VISUAL_QT_FLAG