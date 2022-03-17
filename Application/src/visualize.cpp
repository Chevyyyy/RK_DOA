#include"visualize.hpp"

visualize::visualize(/* args */)
{
    
}
void visualize::visualize_terminal(double theta)
{
    int r = 20;
    int o = 40;
    int locate = r * sin(3.1415926 * ((90 - abs(theta)) / 180)) - o;
    if (locate == (r - o))
    {
        cout << "\n                             -------OOO-----\n";
    }
    else
    {
        cout << "\n                             ---------------\n";
    }

    for (int Y = r - o - 1; Y > -1 * o; Y--)
    {

        int x_axis = sqrt(abs(r * r - (Y + o) * (Y + o)));
        if ((Y != locate) || (theta == -1000))
        {
            cout << setw(o - x_axis) << "------" << setw(2 * x_axis) << "------";
        }
        else if ((theta > 0) && (theta < 90))
        {
            cout << setw(o - x_axis) << "------" << setw(2 * x_axis) << "-OOO-";
        }
        else if ((theta < 0) && (theta > -90))
        {
            cout << setw(o - x_axis) << "-OOO-" << setw(2 * x_axis) << "------";
        }
        else
        {
            cout << setw(o - x_axis) << "------" << setw(2 * x_axis) << "------";
        }

        cout << "\n";
    }
    if (theta == -1000)
    {
        cout << "theta:  "
             << "no sound!"
             << "\n";
    }
    else
    {
        cout << "theta:  " << theta << "\n";
    }
}
void visualize::write_angles_to_txt(double theta, double theta_filtered)
{
    ofstream out("../sound_data/angles.txt");
    if(out.is_open())
    {
        out << theta<<endl<<theta_filtered;
        out.close();
    }
}
void visualize::visualize_qt()
{
    system(" ./../visualize_qt/visualize_qt &");
}
