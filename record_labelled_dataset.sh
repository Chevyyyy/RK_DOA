# name="5s_reverbervation_small_room.wav"
name="10s_-80deg_speech_large_room.wav"


adb shell "arecord -D route_2_2 -c 4 -r 48000 -d 10  -f  S16_LE  /CHEVY_FYP/sound_data/drag/${name}"
# sleep 13s
# adb pull CHEVY_FYP/sound_data/drag/${name} /home/chevy/Desktop/FYP_desk/RK_DOA/sound_data/labelled_dataset
