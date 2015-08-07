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

	ifstream urandom("/dev/urandom");
	ParticleFilter pf(100,&urandom);
	pf.rangeReset(65.0,105.0,65.0,105.0,0.0,360.0);
	pf.print(&ofs);

	ofstream memo("/tmp/memo");

	while(1){
		string action;
		int val;
		cin >> action >> val;
		if(!cin)
			break;

		if(action == "turn"){
			Actions::turn(val);
			pf.motionUpdate(0.0,(double)val);
		}else if(action == "forward"){
			Actions::forward(val);
			pf.motionUpdate((double)val,0.0);
		}

		pf.sensorUpdate();

		pf.print(&ofs);
		usleep(100000);
	}
	
	exit(0);
}
