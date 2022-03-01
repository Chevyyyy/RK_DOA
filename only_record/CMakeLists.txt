cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_COMPILER "/root/prj/Firefly-RK3308/buildroot/output/firefly_rk3308_release/host/bin/aarch64-rockchip-linux-gnu-c++")
set(CMAKE_C_COMPILER "/root/prj/Firefly-RK3308/buildroot/output/firefly_rk3308_release/host/bin/aarch64-rockchip-linux-gnu-gcc")
project(record)

### C++ config
set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_SYSTEM_NAME Android)




add_executable(record_rk main.cpp)


 




