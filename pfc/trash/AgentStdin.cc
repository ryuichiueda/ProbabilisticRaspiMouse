#include "AgentStdin.h"
#include <iostream>
#include "ParticleFilterGyro.h"
#include <cmath>
#include "Actions.h"
#include "SensorGyro.h"
#include "SensorRange.h"
#include <fstream>
#include <unistd.h>
using namespace std;

AgentStdin::AgentStdin(int argc, char const* argv[]) : Agent(argc,argv)
{
	//とりあえず最初に2回読みだしておく
	SensorGyro::update();
	SensorGyro::update();

	m_map_name = argv[1];
}

AgentStdin::~AgentStdin()
{
}

void AgentStdin::doAction(void)
{
	ifstream urandom("/dev/urandom");
	ParticleFilterGyro pf(1000,&urandom,m_map_name);

//	pf.pointReset(180.0*3+90.0,90.0,90.0,5.0,3.0); //consideration of 5mm, 3deg error on placement
	//pf.rangeReset(30.0,180.0*4-30.0,30.0,180.0*4-30.0,0.0,360.0);
	
	//wrong
//	pf.pointReset(180.0*3+90.0,90.0,270.0,5.0,3.0); //consideration of 5mm, 3deg error on placement
//	slightly different
	pf.pointReset(180.0*3+90.0,90.0,75.0,1.0,1.0);

	ofstream ofs("/tmp/particles");
	pf.print(&ofs);

        while(1){
		SensorRange::update();
		pf.sensorUpdate();
//	pf.print(&ofs);
// exit(0);

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
                	pf.print(&ofs);

			double add = val - diff;	
			if(add >= 180.0)	add -= 360.0;
			if(add <= -180.0)	add += 360.0;
			if(fabs(add > 1.0)){
                        	Actions::turn(add);
     	              		SensorGyro::update();
       	               		diff = SensorGyro::getDeltaDeg();
                       		pf.motionUpdate(0.0,0.0,diff);
                		pf.print(&ofs);
			}

                }else if(action == "forward"){
                        Actions::forward(val);
                        SensorGyro::update();
                        double diff = SensorGyro::getDeltaDeg();

                        if(diff < 1.0){
                                pf.motionUpdate((double)val,0.0,diff);
                		pf.print(&ofs);
                        }else{
                                double rad = diff * 3.141592 / 180;
                                double fw = val*sin(rad)/rad;
                                double side = val*(1.0 - cos(rad))/rad;
                                pf.motionUpdate(fw,side,diff);
                		pf.print(&ofs);

	                        Actions::turn(-diff);
       	                	SensorGyro::update();
       	                	diff = SensorGyro::getDeltaDeg();
                        	pf.motionUpdate(0.0,0.0,diff);
                		pf.print(&ofs);
                        }
                }

                usleep(100000);
        }
	exit(0);
}

