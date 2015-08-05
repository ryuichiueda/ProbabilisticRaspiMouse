#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "Actions.h"
#include <unistd.h>
using namespace std;

int main(int argc, char const* argv[])
{
	ofstream ofs("/tmp/deadrec");
	int step = 0;
	double x_mm = 0.0;
	double y_mm = 0.0;
	double t_deg = 90.0;
	ofs << step << " " <<  x_mm << " " << y_mm << " " << t_deg << endl;

	while(1){
		string action;
		int val;
		cin >> action >> val;
		if(!cin)
			break;

		step++;

		if(action == "turn"){
			Actions::turn(val);
			t_deg += val;
		}else if(action == "forward"){
			Actions::forward(val);
			x_mm += val * cos(t_deg * 3.141592 / 180);
			y_mm += val * sin(t_deg * 3.141592 / 180);

		}else if(action == "sleep"){
			usleep(val);
		}
		ofs << step << " " <<  x_mm << " " << y_mm << " " << t_deg << endl;
	}
	
	exit(0);
}
