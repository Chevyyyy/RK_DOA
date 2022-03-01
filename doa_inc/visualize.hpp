#ifndef _VISUALIZE_
#define _VISUALIZE_
#include"iostream"
#include<cmath>
#include<iomanip>
#include<fstream>
#include"stdlib.h"
using namespace std;

class visualize
{
private:
    /* data */
public:
    visualize(/* args */);
    ~visualize(){};
    void visualize_terminal(double theta);
    void visualize_qt();
    void write_angles_to_txt(double theta);
};

#endif