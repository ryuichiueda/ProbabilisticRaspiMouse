#include "AgentAvg.h"
#include <iostream>
#include "ParticleFilterGyro.h"
#include <cmath>
#include "Actions.h"
#include "SensorGyro.h"
#include "SensorRange.h"
#include <fstream>
#include <unistd.h>
using namespace std;

AgentAvg::AgentAvg(int argc, char const* argv[]) : Agent(argc,argv)
{
	//とりあえず最初に2回読みだしておく
	SensorGyro::update();
	SensorGyro::update();

	m_map_name = argv[1];

	ifstream policy_file(argv[2]);
	m_cell_width_mm = 20;
	m_cell_width_deg = 5;
	m_cell_num_x = 36;
	m_cell_num_y = 27;
	m_cell_num_t = 72;

	m_policy.assign(m_cell_num_x*m_cell_num_y*m_cell_num_t,"");

	int state_num;
	string action;
	while(1){
		policy_file >> state_num >> action; 
		if(policy_file.eof())
			break;
		m_policy[state_num] = action;
	}
}

AgentAvg::~AgentAvg()
{
}

void AgentAvg::doAction(void)
{
	ifstream urandom("/dev/urandom");
	ParticleFilterGyro pf(1000,&urandom,m_map_name);

	//pf.rangeReset(40.0,180.0*4-40.0,40.0,180*3-40.0,0.0,359.9);
/*void ParticleFilter::rangeReset(double x_mm_min,double x_mm_max,
				double y_mm_min,double y_mm_max,
				double t_deg_min,double t_deg_max)
*/
	pf.pointReset(180.0*3+90.0,90.0,90.0,5.0,3.0); //consideration of 5mm, 3deg error on placement

	ofstream ofs("/tmp/particles");
	pf.print(&ofs);

	SensorGyro::update();


        while(1){
                SensorRange::update();
                pf.sensorUpdate();

                Particle p = pf.getAverage();
		string str = getAction(p.x_mm,p.y_mm,p.t_rad/3.141592*180.0);
		cerr << "a:" << str << endl;

		if(str == "fw"){
                        Actions::forward(40);
                        SensorGyro::update();
                        double diff = SensorGyro::getDeltaDeg();
                        if(diff < 1.0)
                                pf.motionUpdate(40.0,0.0,diff);
                        else{
                                double rad = diff * 3.141592 / 180;
                                double fw = 40.0*sin(rad)/rad;
                                double side = 40.0*(1.0 - cos(rad))/rad;
                                pf.motionUpdate(fw,side,diff);
                        }

		}else if(str == "cw"){
                        Actions::turn(-5);
                        SensorGyro::update();
                        double diff = SensorGyro::getDeltaDeg();
                        pf.motionUpdate(0.0,0.0,diff);
		}else if(str == "ccw"){
                        Actions::turn(5);
                        SensorGyro::update();
                        double diff = SensorGyro::getDeltaDeg();
                        pf.motionUpdate(0.0,0.0,diff);
		}else{
			break;
		}
			
                pf.print(&ofs);
                cerr << p.x_mm << '\t' << p.y_mm << '\t' << p.t_rad/3.141592*180.0 << endl;
                usleep(100000);
	}
	
	exit(0);
}

string AgentAvg::getAction(double x_mm,double y_mm,double theta_deg)
{
	int ix = floor(x_mm / m_cell_width_mm);
	int iy = floor(y_mm / m_cell_width_mm);
	int it = floor((theta_deg + (double)m_cell_width_deg/2)
			/ m_cell_width_deg);

	if(ix < 0 || ix >= m_cell_num_x)
		return "";
	if(iy < 0 || iy >= m_cell_num_y)
		return "";

	while(it < 0)
		it += m_cell_num_t;
	while(it >= m_cell_num_t)
		it -= m_cell_num_t;

	int index = it + m_cell_num_t*ix + m_cell_num_t*m_cell_num_x*iy;

	return m_policy[index];
}
