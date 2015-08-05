#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
#include "ParticleFilter.h"
#include <unistd.h>
using namespace std;

int main(int argc, char const* argv[])
{
	ofstream ofs("/tmp/particles");
	int step = 0;
	double x_mm = 0.0;
	double y_mm = 0.0;
	double t_deg = 90.0;
	ofs << step << " " <<  x_mm << " " << y_mm << " " << t_deg << endl;

	ifstream urandom("/dev/urandom");
	ParticleFilter pf(10,&urandom);
	pf.pointReset(x_mm,y_mm,t_deg);
	ofs << "step 0" << endl;
	pf.print(&ofs);

	while(1){
		string action;
		int val;
		cin >> action >> val;
		if(!cin)
			break;

		step++;

		if(action == "turn"){
			Actions::turn(val);
			pf.motionUpdate(0.0,(double)val);
		}else if(action == "forward"){
			Actions::forward(val);
			pf.motionUpdate((double)val,0.0);
		}else if(action == "sleep"){
			usleep(val);
		}

		ofs << "step " << step << endl;
		pf.print(&ofs);
	}
	
	exit(0);
}
