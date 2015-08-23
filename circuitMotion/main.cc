#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
#include "ActionsWithSensors.h"
#include "SensorRange.h"
#include "SensorGyro.h"
#include "ParticleFilter.h"
#include <unistd.h>
#include <thread>
#include <string>
using namespace std;

void sensing(void)
{
	SensorRange::update();
}

int main(int argc, char const* argv[])
{
/*
	SensorGyro::init();
	while(1){
		SensorGyro::update();
		//SensorGyro::print();
	}
*/

	ofstream ofs("/tmp/particles");
	ofstream log("/tmp/log");

	ifstream urandom("/dev/urandom");
	ParticleFilter pf(1000,&urandom,argv[1]);
//	pf.rangeReset(30.0,180.0*4-30.0,30.0,180.0*4-30.0,0.0,360.0);
	pf.pointReset(180.0*3+90.0,90.0,180.0,20.0,5.0);
	pf.print(&ofs);

	time_t start = time(NULL);
	int t = time(NULL) - start;

	while(1){
		ActionsWithSensors::findOpenSpace();
		sleep(0.3);
		ActionsWithSensors::forwardToWall();
		sleep(0.3);
/*
		ActionsWithSensors::rightAngle();
		sleep(3);
		//ActionsWithSensors::findOpenSpace();
		Actions::turn(-90);
		sleep(3);
		ActionsWithSensors::forwardToWall();
		sleep(3);
*/
	}
/*
	while(1){
		ActionsWithSensors::forwardToWall();
		ActionsWithSensors::findOpenSpace();
		Actions::forward(150);

		t = time(NULL) - start;
	}
*/

	exit(0);
}
