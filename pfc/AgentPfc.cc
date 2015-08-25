#include "AgentPfc.h"
#include <iostream>
#include "ParticleFilterGyro.h"
#include <cmath>
#include "Actions.h"
#include "SensorGyro.h"
#include "SensorRange.h"
#include <fstream>
#include <unistd.h>
#include <limits.h>
using namespace std;

AgentPfc::AgentPfc(int argc, char const* argv[],int power) : Agent(argc,argv)
{
	//とりあえず最初に2回読みだしておく
	SensorGyro::update();
	SensorGyro::update();

	m_map_name = argv[1];

	ifstream value_file(argv[3]);
	m_cell_width_mm = 20;
	m_cell_width_deg = 5;
	m_cell_num_x = 36;
	m_cell_num_y = 27;
	m_cell_num_t = 72;

        m_init_x_mm = 180.0*3+90.0;
        m_init_y_mm = 90.0;
        m_init_t_deg = 90.0;

        if(argc == 7){
                m_init_x_mm = (double)atof(argv[4]);
                m_init_y_mm = (double)atof(argv[5]);
                m_init_t_deg = (double)atof(argv[6]);

                cerr << m_init_x_mm << ' ' << m_init_y_mm << ' ' << m_init_t_deg << endl;
        }

	m_value.assign(m_cell_num_x*m_cell_num_y*m_cell_num_t,UINT_MAX);

	int state_num = 0;
	unsigned long value = 0;
	while(1){
		value_file >> state_num >> value;
		if(value_file.eof())
			break;
		m_value[state_num] = value;
	}

/*
	for(i=0;i<state_num;i++)
		cerr << m_value[i] << endl;
*/

	m_power = power;

}

AgentPfc::~AgentPfc()
{
}

void AgentPfc::doAction(void)
{
        ifstream urandom("/dev/urandom");
        ParticleFilterGyro pf(1000,&urandom,m_map_name);

	//pf.pointReset(m_init_x_mm,m_init_y_mm,m_init_t_deg,5.0,3.0); //consideration of 5mm, 3deg error on placement
	pf.randomReset();

        ofstream ofs("/tmp/particles");
        pf.print(&ofs);

        SensorGyro::update();

        while(1){
                SensorRange::update();
                pf.sensorUpdate();

                Particle p = pf.getAverage();
		string str = getAction(&pf);
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
			if(diff >= 180.0)	diff -= 360.0;
			else if(diff <= -180.0)	diff += 360.0;

                        pf.motionUpdate(0.0,0.0,diff);
			double error = -5.0 - diff;
                        Actions::turn(error*2);
                        SensorGyro::update();
                        diff = SensorGyro::getDeltaDeg();
			if(diff >= 180.0)	diff -= 360.0;
			else if(diff <= -180.0)	diff += 360.0;

                        pf.motionUpdate(0.0,0.0,diff);
		}else if(str == "ccw"){
                        Actions::turn(5);
                        SensorGyro::update();
                        double diff = SensorGyro::getDeltaDeg();
			if(diff >= 180.0)	diff -= 360.0;
			else if(diff <= -180.0)	diff += 360.0;

                        pf.motionUpdate(0.0,0.0,diff);
			double error = 5.0 - diff;
                        Actions::turn(error*2);
                        SensorGyro::update();
                        diff = SensorGyro::getDeltaDeg();
			if(diff >= 180.0)	diff -= 360.0;
			else if(diff <= -180.0)	diff += 360.0;

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


double AgentPfc::getValue(double x_mm,double y_mm,double theta_deg)
{
	int ix = floor(x_mm / m_cell_width_mm);
	int iy = floor(y_mm / m_cell_width_mm);
	int it = floor((theta_deg + (double)m_cell_width_deg/2)
			/ m_cell_width_deg);

	if(ix < 0 || ix >= m_cell_num_x)
		return UINT_MAX;
	if(iy < 0 || iy >= m_cell_num_y)
		return UINT_MAX;

	while(it < 0)
		it += m_cell_num_t;
	while(it >= m_cell_num_t)
		it -= m_cell_num_t;

	int index = it + m_cell_num_t*ix + m_cell_num_t*m_cell_num_x*iy;

	double v = m_value[index];
	return v;
}

void AgentPfc::stateTransition(double x_mm,double y_mm,double theta_rad,
				double fw, double rad,
				double *ax_mm,double *ay_mm,double *atheta_rad)
{
	*ax_mm = x_mm + fw * cos(theta_rad);
	*ay_mm = y_mm + fw * sin(theta_rad);
	*atheta_rad = theta_rad + rad;
}

string AgentPfc::getAction(ParticleFilterGyro *pf)
{
	int outcounter1 = 0;
	int outcounter2 = 0;
	for(auto &p : pf->m_particles){
		int prev_i = getIndex(p.x_mm,p.y_mm,p.t_rad/3.141592*180.0);
		if(prev_i >= 0){
			if(m_value[prev_i] == 0)
				p.w *= 0.00001;
			else if(m_value[prev_i] > 10100){
				outcounter1++;
			}
		}else{
			outcounter2++;
		}
	}

	cerr << outcounter1 << '\t' << outcounter2 << endl;
	
/*
	if(!pf->reset())
		pf->resampling();
*/

	double v_sum[3];
	//fw evaluation
	v_sum[0] = 0.0;
	for(auto p : pf->m_particles){
		int prev_i = getIndex(p.x_mm,p.y_mm,p.t_rad/3.141592*180.0);
		unsigned int prev_v = 10200;
		if(prev_i >= 0)
			prev_v = m_value[prev_i];
		if(prev_v == 0)
			continue;

		double x = p.x_mm + 40.0*cos(p.t_rad);
		double y = p.y_mm + 40.0*sin(p.t_rad);
		int i = getIndex(x,y,p.t_rad/3.141592*180.0);
		if(i >= 0){
			if(m_value[i] > 10100)
				v_sum[0] += prev_v*p.w/pow(prev_v,m_power);
			else
				v_sum[0] += m_value[i]*p.w/pow(prev_v,m_power);
		}else
			v_sum[0] += prev_v*p.w/pow(prev_v,m_power);
	}

	//cw evaluation
	v_sum[1] = 0.0;
	for(auto p : pf->m_particles){
		int prev_i = getIndex(p.x_mm,p.y_mm,p.t_rad/3.141592*180.0);
		unsigned int prev_v = 10200;
		if(prev_i >= 0)
			prev_v = m_value[prev_i];
		if(prev_v == 0)
			continue;

		int i = getIndex(p.x_mm,p.y_mm,(p.t_rad/3.141592*180.0 - 5.0));
		if(i >= 0)
			v_sum[1] += m_value[i]*p.w/pow(prev_v,m_power);
		else
			v_sum[1] += 10200.0*p.w/pow(prev_v,m_power);
	}
	//ccw evaluation
	v_sum[2] = 0.0;
	for(auto p : pf->m_particles){
		int prev_i = getIndex(p.x_mm,p.y_mm,p.t_rad/3.141592*180.0);
		unsigned int prev_v = 10200;
		if(prev_i >= 0)
			prev_v = m_value[prev_i];
		if(prev_v == 0)
			continue;

		int i = getIndex(p.x_mm,p.y_mm,(p.t_rad/3.141592*180.0 + 5.0));
		if(i >= 0)
			v_sum[2] += m_value[i]*p.w/pow(prev_v,m_power);
		else
			v_sum[2] += 10200.0*p.w/pow(prev_v,m_power);
	}

	cerr << v_sum[0] << '\t' << v_sum[1] << '\t' << v_sum[2] << endl;
	if(v_sum[0] > 1.0 && v_sum[1] > 1.0 && v_sum[2] > 1.0)
		return "fw";

	if(v_sum[0] <= v_sum[1] && v_sum[0] <= v_sum[2])
		return "fw";

	if(v_sum[1] <= v_sum[2])
		return "cw";

	return "ccw";
}

int AgentPfc::getIndex(double x_mm,double y_mm,double theta_deg)
{
        int ix = floor(x_mm / m_cell_width_mm);
        int iy = floor(y_mm / m_cell_width_mm);
        int it = floor((theta_deg + (double)m_cell_width_deg/2)
                        / m_cell_width_deg);

        if(ix < 0 || ix >= m_cell_num_x)
                return -1;
        if(iy < 0 || iy >= m_cell_num_y)
                return -1;

        while(it < 0)
                it += m_cell_num_t;
        while(it >= m_cell_num_t)
                it -= m_cell_num_t;

        return it + m_cell_num_t*ix + m_cell_num_t*m_cell_num_x*iy;
}
