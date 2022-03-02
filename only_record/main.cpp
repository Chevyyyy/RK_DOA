#include"stdlib.h"
#include"iostream"
#include"fstream"
#include"unistd.h"
#include"wav_decode.hpp"

using namespace std;

int main()
{
	cout << "ready for recording" << endl;
	string a = "finished";
	
	for (int i = 1; i < 1e10; i++)
	{
		
		
		while(a!="finished")
		{
			ifstream f("/CHEVY_FYP/communication_log");
			f >> a;
			f.close();
			// usleep(1000);
		}

		string cmd = "arecord -D hw:0,0 -c 8 -r 44100 -s " + to_string(RANGE+15)+ "-f  S16_LE  --period-size=1024  --buffer-size=4096 /CHEVY_FYP/test.wav &>a.log";
		system("arecord -D hw:0,0 -c 8 -r 44100 -s 110   -f  S16_LE  --period-size=1024  --buffer-size=4096 /CHEVY_FYP/test.wav &>a.log");
		cout << i << " recording" << endl;
		a = "waiting";
		system("echo waiting > /CHEVY_FYP/communication_log");
		// system("clear");
	}



	return 0;
	
}
