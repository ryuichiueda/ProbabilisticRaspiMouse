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

////	pf.pointReset(180.0*3+90.0,90.0,90.0,5.0,3.0); //consideration of 5mm, 3deg error on placement
	pf.rangeReset(30.0,180.0*4-30.0,30.0,180.0*4-30.0,0.0,360.0);

	ofstream ofs("/tmp/particles");
	pf.print(&ofs);

	SensorRange::update();
	pf.sensorUpdate();
	pf.print(&ofs);

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


/*
        for(int i=0;i<8;i++){
		cerr << i << endl;
                SensorRange::update();
                pf.sensorUpdate();

                if(i==0 || i==2 || i==3 || i==5){
                        Actions::forward(180);
                        SensorGyro::update();
                        double diff = SensorGyro::getDeltaDeg();
                        pf.motionUpdate(180.0,0.0,diff);
                }else if(i==1 || i==4 || i==6 || i==7){
                        Actions::turn(90);
                        SensorGyro::update();
                        double diff = SensorGyro::getDeltaDeg();
                        pf.motionUpdate(0.0,0.0,diff);
                }
                Particle p = pf.getAverage();
                pf.print(&ofs);
                cerr << p.x_mm << '\t' << p.y_mm << '\t' << p.t_rad/3.141592*180.0 << endl;
        }
*/
	
	exit(0);
}

