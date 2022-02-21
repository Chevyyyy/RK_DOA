//         


// void visualize(double theta)
// {

//     int r = 20;
//     int o = 40;
//     int locate = r * sin(3.1415926 * ((90 - abs(theta)) / 180)) - o;
//     if (locate == (r - o))
//     {
//         cout << "\n                             -------OOO-----\n";
//     }
//     else
//     {
//         cout << "\n                             ---------------\n";
//     }

//     for (int Y = r - o - 1; Y > -1 * o; Y--)
//     {

//         int x_axis = sqrt(abs(r * r - (Y + o) * (Y + o)));
//         if ((Y != locate) || (theta == -1000))
//         {
//             cout << setw(o - x_axis) << "------" << setw(2 * x_axis) << "------";
//         }
//         else if ((theta > 0) && (theta < 90))
//         {
//             cout << setw(o - x_axis) << "------" << setw(2 * x_axis) << "-OOO-";
//         }
//         else if ((theta < 0) && (theta > -90))
//         {
//             cout << setw(o - x_axis) << "-OOO-" << setw(2 * x_axis) << "------";
//         }
//         else
//         {
//             cout << setw(o - x_axis) << "------" << setw(2 * x_axis) << "------";
//         }

//         cout << "\n";
//     }
//     if (theta == -1000)
//     {
//         cout << "theta:  "
//              << "no sound!"
//              << "\n";
//     }
//     else
//     {
//         cout << "theta:  " << theta << "\n";
//     }
// }


// void delay_print()
// {
//     cout << "\n\n\n\n\n";

//     cout << "####################################\n";
//     cout << "####################################\n";
//     cout << "max_cc12:" << delay1234.delay12.cc << "\n";
//     cout << "true_delay12:" << delay1234.delay12.delay << "\n";
//     cout << "####################################\n";
//     cout << "max_cc13:" << delay1234.delay13.cc << "\n";
//     cout << "true_delay13:" << delay1234.delay13.delay << "\n";
//     cout << "####################################\n";
//     cout << "max_cc14:" << delay1234.delay14.cc << "\n";
//     cout << "true_delay14:" << delay1234.delay14.delay << "\n";
//     cout << "####################################\n";
//     cout << "true_delay23:" << delay1234.delay23.delay << "\n";
//     cout << "max_cc23:" << delay1234.delay23.cc << "\n";
//     cout << "####################################\n";
//     cout << "max_cc24:" << delay1234.delay24.cc << "\n";
//     cout << "true_delay24:" << delay1234.delay24.delay << "\n";
//     cout << "####################################\n";
//     cout << "max_cc34:" << delay1234.delay34.cc << "\n";
//     cout << "true_delay34:" << delay1234.delay34.delay << "\n";
//     cout << "####################################\n";
//     cout << "####################################\n";
//     cout << "####################################\n";

//     cout << "theta:  " << theta << "\n";
// }

