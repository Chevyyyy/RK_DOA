

void rk_config();       

//@brief: basic constant
#define LPF_CUTOFF 100
#define CHANNEL_NUM 4
#define mic_distance 0.0345
#define Vs 340.0 // speed of sound
#define RANGE 512*2
#define SAMPLE_RATE 48000
#define SHOW_RAW_DATA 0
#define DELAY_MAX mic_distance*SAMPLE_RATE / Vs 
#define PI 3.1415926
#define Degree_To_Rad(degree) degree *PI / 180.0

//@brief: mode
#define Track_speech_flag
// #define ON_RKCHIP_FLAG

//@brief: MATH algorithm & hyperparameters
#define PHAT_SPR



//@brief: parameters (testing)
#define Usleep_time 7000
// #define VISUAL_QT_FLAG

