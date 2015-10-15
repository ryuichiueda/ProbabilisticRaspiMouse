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
	ParticleFilterGyro m_pf(1000,&urandom,m_map_name);

	m_pf.pointReset(180.0*3+90.0,90.0,90.0,5.0,3.0); //consideration of 5mm, 3deg error on placement

	ofstream ofs("/tmp/particles");
	m_pf.print(&ofs);

	SensorGyro::update();


        for(int i=0;i<8;i++){
		cerr << i << endl;
                SensorRange::update();
                m_pf.sensorUpdate();

                if(i==0 || i==2 || i==3 || i==5){
                        Actions::forward(180);
                        SensorGyro::update();
                        double diff = SensorGyro::getDeltaDeg();
                        m_pf.motionUpdate(180.0,0.0,diff);
                }else if(i==1 || i==4 || i==6 || i==7){
                        Actions::turn(90);
                        SensorGyro::update();
                        double diff = SensorGyro::getDeltaDeg();
                        m_pf.motionUpdate(0.0,0.0,diff);
                }
                Particle p = m_pf.getAverage();
                m_pf.print(&ofs);
                cerr << p.x_mm << '\t' << p.y_mm << '\t' << p.t_rad/3.141592*180.0 << endl;
        }
	
	exit(0);
}

string AgentStdin::getAction(double x_mm,double y_mm,double theta_deg)
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
