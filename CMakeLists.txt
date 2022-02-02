cmake_minimum_required(VERSION 3.0)

project(RK_DOA)

add_executable(rk_doa_cmake main.cpp )

include_directories(include)

#set(CMAKE_CXX_COMPILER "/root/prj/Firefly-RK3308/buildroot/output/firefly_rk3308_release/host/bin/aarch64-rockchip-linux-gnu-c++")

execute_process(COMMAND make)