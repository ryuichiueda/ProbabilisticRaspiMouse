#include "DeadRecMonteCarlo.h"
#include <iostream>
#include <limits.h>
#include <cmath>
using namespace std;

DeadRecMonteCarlo::DeadRecMonteCarlo(int num, ifstream *ifs)
{
	Pose p{0.0,0.0,0.0};
	m_poses.insert(m_poses.begin(),num,p);

	m_rand_ifs = ifs;
	m_step = 0;

	m_distance_max_noise_ratio = 0.05; // 5% noise
	m_direction_max_noise_ratio = 0.20; // 20% noise
}

DeadRecMonteCarlo::~DeadRecMonteCarlo()
{
}

void DeadRecMonteCarlo::motionUpdate(double fw_delta_mm,double t_delta_deg)
{
	double t_delta_rad = t_delta_deg * 3.141592 / 180;

	for(auto &p : m_poses){
		//xy方向
		double pos_noise = m_distance_max_noise_ratio * getDoubleRand();
		double pos_noise_angle = getDoubleRand() * 2 * 3.141592;
		double x_noise = pos_noise * cos(pos_noise_angle);
		double y_noise = pos_noise * sin(pos_noise_angle);

		p.x_mm += fw_delta_mm * (cos(p.t_rad) + x_noise);
		p.y_mm += fw_delta_mm * (sin(p.t_rad) + y_noise);

		//theta方向
		double ratio = m_direction_max_noise_ratio * (2*getDoubleRand()-1.0);
		p.t_rad += t_delta_rad * (1.0 + ratio);
	}
	m_step++;
}

void DeadRecMonteCarlo::pointReset(double x_mm,double y_mm,double t_deg,
				double pos_max_noise_mm,double dir_max_noise_deg)
{
	for(auto &p : m_poses){
		double dir_noise = dir_max_noise_deg *(2*getDoubleRand() - 1.0);

		double pos_noise = pos_max_noise_mm * getDoubleRand();
		double pos_noise_angle = getDoubleRand() * 2 * 3.141592;
		double x_noise = pos_noise * cos(pos_noise_angle);
		double y_noise = pos_noise * sin(pos_noise_angle);

		p = {x_mm+x_noise, y_mm+y_noise, (t_deg + dir_noise)/180*3.141592};
	}
}

// from 0 to UINT_MAX
unsigned int DeadRecMonteCarlo::getIntRand()
{
	char buf[4];
	m_rand_ifs->read(buf,4);
	return (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
}

// from 0.0 to 1.0
double DeadRecMonteCarlo::getDoubleRand()
{
	return (double)getIntRand() / UINT_MAX;
}

void DeadRecMonteCarlo::print(ofstream *ofs)
{
	for(auto &p : m_poses){
		*ofs << m_step << " " << (int)p.x_mm << " " << (int)p.y_mm << " "
			<< (int)(p.t_rad / 3.141592 * 180) << endl;
	}
}
