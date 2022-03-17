#ifndef  _FIND_MICS_DELAY_
#define _FIND_MICS_DELAY

using namespace std;



int CC(Wave1234 wx, Wave1234 wy, int n)
{
   int16_t x[range];
   int16_t y[range];
   int volume=0;
   for (int tt = 0; tt < 6; tt++)
   {
      switch (tt)
      {
      case 0:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch1[i];
             y[i] = wy.ch2[i];
         }
         break;
      case 1:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch1[i];
             y[i] = wy.ch3[i];
         }
         break;
      case 2:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch1[i];
             y[i] = wy.ch4[i];
         }
         break;
      case 3:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch2[i]; y[i] = wy.ch3[i];
         }
         break;
      case 4:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch2[i];
             y[i] = wy.ch4[i];
         }
         break;
      case 5:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch3[i];
            y[i] = wy.ch4[i];
         }
         break;
      default:
         break;
      }

      int i, j;
      double mx, my, sx, sy, sxy, denom, r;

      /* Calculate the mean of the two series x[], y[] */
      mx = 0;
      my = 0;

      for (i = 0; i < n; i++)
      {
         // cout<<x[i]<<"\n";
         // cout<<y[i]<<"\n";
         mx += x[i];
         my += y[i];
         // volume=volume+abs(x[i])+abs(y[i]);
      }


      mx /= n;
      my /= n;

      /* Calculate the denominator */
      sx = 0;
      sy = 0;
      for (i = 0; i < n; i++)
      {
         sx += (x[i] - mx) * (x[i] - mx);
         sy += (y[i] - my) * (y[i] - my);
   
      }
            // cout<<"##############"<<sx<<"############"<<endl;
      
      denom = sqrt(sx * sy);
      // cout << denom << "\n";
      double true_delay = 0;
      double max_cc = 0;
      int maxdelay = 5;
      /* Calculate the correlation series */
      //move y to get the delay
      for (int delay = -maxdelay; delay < maxdelay; delay++)
      {
         sxy = 0;
         for (i = maxdelay; i < n - maxdelay; i++)
         {
            j = i + delay;
            if (j < 0 || j >= n)
               continue;
            else
               sxy += (x[i] - mx) * (y[j] - my);
            /* Or should it be (?)
         if (j < 0 || j >= n)
            sxy += (x[i] - mx) * (-my);
         else
            sxy += (x[i] - mx) * (y[j] - my);
         */
         }
         r = sxy / denom;

         if (r > max_cc)
         {
            max_cc = r;
            true_delay = delay;
         }
         volume=denom;
         switch (tt)
      {
      case 0:
         delay1234.delay12.cc = max_cc;
         delay1234.delay12.delay = true_delay;
         break;
      case 1:
         delay1234.delay13.cc = max_cc;
         delay1234.delay13.delay = true_delay;
         break;
      case 2:
                  delay1234.delay14.cc = max_cc;
         delay1234.delay14.delay = true_delay;
         break;
      case 3:
         delay1234.delay23.cc = max_cc;
         delay1234.delay23.delay = true_delay;
         break;
      case 4:
                 delay1234.delay24.cc = max_cc;
         delay1234.delay24.delay = true_delay;
         break;
      case 5:
         delay1234.delay34.cc = max_cc;
         delay1234.delay34.delay = true_delay;
         break;
      default:
         break;
      }


         /* r is the correlation coefficient at "delay" */
      }
   }
   
 return volume;
}
int CC1(Wave1234 wx, Wave1234 wy, int n)
{
   int16_t x[range];
   int16_t y[range];
   int volume=0;
   for (int tt = 0; tt < 6; tt++)
   {
      switch (tt)
      {
      case 0:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch1[i];
             y[i] = wy.ch2[i];
         }
         break;
      case 1:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch1[i];
             y[i] = wy.ch3[i];
         }
         break;
      case 2:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch1[i];
             y[i] = wy.ch4[i];
         }
         break;
      case 3:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch2[i]; y[i] = wy.ch3[i];
         }
         break;
      case 4:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch2[i];
             y[i] = wy.ch4[i];
         }
         break;
      case 5:
         for (int i = 0; i < range; i++)
         {
            x[i] = wx.ch3[i];
            y[i] = wy.ch4[i];
         }
         break;
      default:
         break;
      }

      int i, j;
      double mx, my, sx, sy, sxy, denom, r;

      /* Calculate the mean of the two series x[], y[] */
      mx = 0;
      my = 0;


      double true_delay = 0;
      double max_cc = 0;
      int maxdelay = 10;
      /* Calculate the correlation series */
      //move y to get the delay
      for (int delay = -maxdelay; delay < maxdelay; delay++)
      {
         sxy = 0;
         for (i = maxdelay; i < n - maxdelay; i++)
         {
            j = i + delay;
            if (j < 0 || j >= n)
               continue;
            else
               sxy += x[i] *y[j];
            /* Or should it be (?)
         if (j < 0 || j >= n)
            sxy += (x[i] - mx) * (-my);
         else
            sxy += (x[i] - mx) * (y[j] - my);
         */
         }
         r = sxy;

         if (r > max_cc)
         {
            max_cc = r;
            true_delay = delay;
         }
         switch (tt)
      {
      case 0:
         delay1234.delay12.cc = max_cc;
         delay1234.delay12.delay = true_delay;
         break;
      case 1:
         delay1234.delay13.cc = max_cc;
         delay1234.delay13.delay = true_delay;
         break;
      case 2:
                  delay1234.delay14.cc = max_cc;
         delay1234.delay14.delay = true_delay;
         break;
      case 3:
         delay1234.delay23.cc = max_cc;
         delay1234.delay23.delay = true_delay;
         break;
      case 4:
                 delay1234.delay24.cc = max_cc;
         delay1234.delay24.delay = true_delay;
         break;
      case 5:
         delay1234.delay34.cc = max_cc;
         delay1234.delay34.delay = true_delay;
         break;
      default:
         break;
      }


         /* r is the correlation coefficient at "delay" */
      }
   }
 return ( delay1234.delay12.delay + delay1234.delay23.delay +  delay1234.delay34.delay + 2 * delay1234.delay13.delay + 2 *  delay1234.delay24.delay + 3 *delay1234.delay14.delay) / 10;
}


void delay_print();


#endif