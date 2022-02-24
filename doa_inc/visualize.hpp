#ifndef _VISUALIZE_
#define _VISUALIZE_
#include"iostream"
#include<cmath>
#include<iomanip>
#include<fstream>
using namespace std;

class visualize
{
private:
    /* data */
public:
    visualize(/* args */);
    ~visualize(){};
    void visualize_terminal(double theta);
    void write_angles_to_txt(double theta);
};

#endif