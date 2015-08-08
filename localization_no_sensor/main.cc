#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
#include "Sensors.h"
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
	ParticleFilter pf(1000,&urandom,argv[1]);
	pf.rangeReset(30.0,180.0*4-30.0,30.0,180.0*4-30.0,0.0,360.0);
	pf.print(&ofs);
///////////////////////////////////////

	bool no_wall = true;
	int val = 5;

	int counter = 0;

	while(1){
		if(no_wall){
			thread async_motion(motion,"forward",30);
			pf.sensorUpdate();
			no_wall = Sensors::noWallFront();
			pf.motionUpdate((double)val,0.0);
			pf.print(&ofs);
			async_motion.join();

			if(!no_wall){
				if(counter%5 == 0)
					val *= -1;
				counter++;
			}
		}else{
			thread async_motion(motion,"turn",val);
			pf.sensorUpdate();
			no_wall = Sensors::noWallFront();
			pf.motionUpdate(0.0,(double)val);
			pf.print(&ofs);
			async_motion.join();
		}
	}
	exit(0);
}
