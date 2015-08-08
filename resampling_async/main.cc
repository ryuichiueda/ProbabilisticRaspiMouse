#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
#include "ParticleFilter.h"
#include <unistd.h>
#include <thread>
using namespace std;

void motion(string action, int val)
{
	if(action == "turn"){
		Actions::turn(val);
	}else if(action == "forward"){
		Actions::forward(val);
	}
}

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

		thread async_motion(motion,action,val);
		pf.sensorUpdate();
		if(action == "turn"){
			pf.motionUpdate(0.0,(double)val);
		}else if(action == "forward"){
			pf.motionUpdate((double)val,0.0);
		}
		pf.print(&ofs);
		async_motion.join();
	}
	
	exit(0);
}
