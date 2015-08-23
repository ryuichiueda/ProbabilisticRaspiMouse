#include "AgentGyro.h"
#include <iostream>
#include "DeadRecMonteCarlo.h"
#include <cmath>
#include "Actions.h"
#include "SensorGyro.h"
#include <fstream>
#include <unistd.h>
using namespace std;

AgentGyro::AgentGyro(int argc, char const* argv[]) : Agent(argc,argv)
{
	//とりあえず最初に2回読みだしておく
	SensorGyro::update();
	SensorGyro::update();
}

AgentGyro::~AgentGyro()
{
}

void AgentGyro::doAction(void)
{
	ofstream ofs("/tmp/particles");
	ifstream urandom("/dev/urandom");
	DeadRecMonteCarlo pf(100,&urandom);
	pf.pointReset(0.0,0.0,90.0,5.0,5.0); //consideration of 5mm, 5deg error on placement
	pf.print(&ofs);

	SensorGyro::update();

	while(1){
		string action;
		int val;
		cin >> action >> val;
		if(!cin)
			break;

		if(action == "turn"){
			Actions::turn(val);
			SensorGyro::update();
			double diff = SensorGyro::getDeltaDeg();
			pf.motionUpdate(0.0,0.0,diff);

		}else if(action == "forward"){
			Actions::forward(val);
			SensorGyro::update();
			double diff = SensorGyro::getDeltaDeg();

			if(diff < 1.0)
				pf.motionUpdate((double)val,0.0,diff);
			else{
				double rad = diff * 3.141592 / 180;
				double fw = val*sin(rad)/rad;
				double side = val*(1.0 - cos(rad))/rad;
				pf.motionUpdate(fw,side,diff);
			}
		}

		pf.print(&ofs);
		usleep(100000);
	}
	
	exit(0);
}
