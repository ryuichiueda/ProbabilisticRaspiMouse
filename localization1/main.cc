#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
#include "Sensors.h"
#include "ParticleFilter.h"
#include <unistd.h>
#include <thread>
#include <string>
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
//	pf.rangeReset(30.0,180.0*4-30.0,30.0,180.0*4-30.0,0.0,360.0);
	pf.pointReset(180.0*3+90.0,90.0,180.0,20.0,5.0);
	pf.print(&ofs);

	string act;
	int lf, ls, rs, rf;
	int val;
	int i = 0;
	while(1){
		if(i%2==0){
			cin >> act >> lf >> ls >> rs >> rf;
			pf.sensorUpdateDebug(lf,rf);
		}else{
			cin >> act >> val;
			if(act == "turn")
				pf.motionUpdate(0.0,(double)val);
			else
				pf.motionUpdate((double)val,0.0);

			pf.print(&ofs);
			
		}
		if(!cin)
			exit(0);
		i++;
	}
/*
///////////////////////////////////////

	bool no_wall = true;
	int val = 5;

	int counter = 0;

	while(1){
		if(no_wall){
			thread async_motion(motion,"forward",30);
			no_wall = Sensors::noWallFront();
			pf.motionUpdate(30.0,0.0);
			pf.print(&ofs);
			async_motion.join();

			if(!no_wall){
				if(counter%5 == 0)
					val *= -1;
				counter++;
			}
		}else{
			thread async_motion(motion,"turn",val);
			no_wall = Sensors::noWallFront();
			pf.motionUpdate(0.0,(double)val);
			pf.print(&ofs);
			async_motion.join();
		}
	}
*/
	exit(0);
}
