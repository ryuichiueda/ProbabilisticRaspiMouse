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

void sensing(void)
{
	Sensors::update();
}

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
	ofstream log("/tmp/log");

	ifstream urandom("/dev/urandom");
	ParticleFilter pf(1000,&urandom,argv[1]);
//	pf.rangeReset(30.0,180.0*4-30.0,30.0,180.0*4-30.0,0.0,360.0);
	pf.pointReset(180.0*3+90.0,90.0,180.0,20.0,5.0);
	pf.print(&ofs);

/*
        string act;
        int lf, ls, rs, rf;
        int val;
        int i = 0;
        while(1){
                if(i%2==0){
                        cin >> lf >> ls >> rs >> rf;
                        pf.sensorUpdateDebug(lf,rf);
                        pf.sensorUpdateDebug2(lf,ls,rs,rf);
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
*/

/*
	for(int k=0;k<18;k++){
		thread async_sensor(sensing);
		thread async_motion(motion,"turn",10);
		async_sensor.join();
	//	pf.sensorUpdate();	
		Sensors::noWallFront();

		pf.motionUpdate(0.0,(double)10);
		Sensors::print(&log);
		log << "turn " << 5 << endl;

		pf.print(&ofs);
		async_motion.join();
	}
	exit(0);
*/

	bool no_wall = true;
	int t_val = 10;

	time_t start = time(NULL);
	int t = time(NULL) - start;
	while(t < 15){
		thread async_sensor(sensing);

		if((t >= 5 && t <= 6) || (!no_wall)){
			thread async_motion(motion,"turn",t_val);
			async_sensor.join();
			pf.sensorUpdate();	
			no_wall = Sensors::noWallFront();

			pf.motionUpdate(0.0,(double)t_val);
			Sensors::print(&log);
			log << "turn " << t_val << endl;

			pf.print(&ofs);
			async_motion.join();
		}else{
			thread async_motion(motion,"forward",30);
			async_sensor.join();
			pf.sensorUpdate();	
			no_wall = Sensors::noWallFront();

			pf.motionUpdate(30.0,0.0);
			Sensors::print(&log);
			log << "forward 30" << endl;

			pf.print(&ofs);
			async_motion.join();

			if(rand()%2==0)
				t_val *= -1;
		}/*else{
			thread async_motion(motion,"turn",t_val);
			async_sensor.join();
			pf.sensorUpdate();	
			no_wall = Sensors::noWallFront();

			pf.motionUpdate(0.0,(double)t_val);
			Sensors::print(&log);
			log << "turn " << t_val << endl;

			pf.print(&ofs);
			async_motion.join();
		}*/
		t = time(NULL) - start;
	}

	start = time(NULL);
	t = time(NULL) - start;
	while(t < 2){
		thread async_sensor(sensing);
		thread async_motion(motion,"turn",t_val);
		async_sensor.join();
		pf.sensorUpdate();	
		no_wall = Sensors::noWallFront();

		pf.motionUpdate(0.0,(double)t_val);
		Sensors::print(&log);
		log << "turn " << t_val << endl;

		pf.print(&ofs);
		async_motion.join();

		t = time(NULL) - start;
	}
	exit(0);
}
