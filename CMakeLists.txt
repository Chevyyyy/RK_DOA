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



### Create Zero GCC-PHAT library
add_library(zo-gcc-phat doa_src/zo-gcc-phat.cpp
                      doa_src/zo-fft-fftw.cpp )
target_include_directories(zo-gcc-phat PRIVATE ${FFTW3_INCLUDES} ./doa_inc)
target_link_libraries(zo-gcc-phat ${FFTW3_LIBRARIES})


### Create main App
AUX_SOURCE_DIRECTORY( doa_src DIR_SRCS)
add_executable(rk_doa_run main.cpp ${DIR_SRCS})
target_include_directories(rk_doa_run PRIVATE ./doa_inc)
target_link_libraries(rk_doa_run zo-gcc-phat)#link GCC lib

 
# set(CMAKE_CXX_COMPILER "/root/prj/Firefly-RK3308/buildroot/output/firefly_rk3308_release/host/bin/aarch64-rockchip-linux-gnu-c++")




