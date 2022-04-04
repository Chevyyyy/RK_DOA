
cmake_minimum_required(VERSION 3.0)
# set(CMAKE_CXX_COMPILER "/root/prj/Firefly-RK3308/buildroot/output/firefly_rk3308_release/host/bin/aarch64-rockchip-linux-gnu-c++")
# set(CMAKE_C_COMPILER "/root/prj/Firefly-RK3308/buildroot/output/firefly_rk3308_release/host/bin/aarch64-rockchip-linux-gnu-gcc")
project(RK_DOA)


### Get the required packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# FFTW
find_package(FFTW3 REQUIRED)


### C++ config
set(CMAKE_CXX_STANDARD 11)  

# set(CMAKE_SYSTEM_NAME Android)
include_directories(. /home/chevy/compile_eigen3_2/arm_eigen/install/include/eigen3)

# link alsa lib
link_libraries(-lasound)


### Create main App
add_executable(rk_doa_run 
main.cpp 
Application/src/visualize.cpp
Application/src/WaveSignalProcess.cpp
Application/src/wav_decode.cpp
Application/src/wav_decode.cpp
Application/src/AlsaCaptureAudio.cpp

MATH/src/zo-gcc-phat.cpp
MATH/src/AudioFFT.cpp
MATH/src/LowPassFilter.cpp
MATH/src/kalman.cpp

)
target_include_directories(rk_doa_run PRIVATE ./MATH/inc ./Application/inc)
# target_link_libraries(rk_doa_run zo-gcc-phat)#link GCC lib

 



