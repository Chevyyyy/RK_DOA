#################################################################
You can  directly used the complied file in the directory of "Release" if no change to the source file
Directly go to step 2 (setup on RKchip)
#################################################################
1. How to compile? (on Linux)  

1.1 main progrom compile

    mkdir build 
    cd build
    cmake ..
    make
    mv rk_doa_run Release

    Then you will get a executable file "rk_doa_run" that can be runned on the RKchip platform

1.2 qt program compile

    cd ..
    cd visualize_qt
    make
    mv visualize_qt Release

    Then you will get a executabel file visualize_qt which can be runned on Linux.


2. setup on RKchip (on RKchip)

2.1 connect the RKchip with Linux with ADB and mkdir a working directory named "CHEVY_FYP"
    
    adb shell 
    mkdir /CHEVY_FYP

2.2 push executable file and config file to the RKchip from Linux terminal

    cd ..
    adb push Release/rk_doa_run CHEVY_FYP
    adb push Release/RK_config CHEVY_FYP
    adb push Release/asound.conf etc



3. Run and visulaization

3.1 run rk_doa_run in RKchip (on RKchip)

    cd CHEVY_FYP
    ./rk_doa_run

3.2 visulize in Linux terminal (on linux)

    cd Release
    ./visualize_qt


4. Tune the parameters on Rkchip through editting RK_config (on RKchip)

    you can open another adb shell terminal for tunning the parameters while running the program 

    cd CHEVY_FYP
    
    vi RK_config
    ......

    edit the parameter in the file and then save the file
    Detail for configuring is in tunning detail for RK_DOA.pdf






















  







  
  

