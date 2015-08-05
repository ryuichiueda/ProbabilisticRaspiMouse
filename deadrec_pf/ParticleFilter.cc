#include "ParticleFilter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits.h>
#include <cmath>
using namespace std;

ParticleFilter::ParticleFilter(int num, ifstream *ifs)
{
	Particle p{0.0,0.0,0.0,1.0/num};
	m_particles.insert(m_particles.begin(),num,p);

	m_rand_ifs = ifs;
}

ParticleFilter::~ParticleFilter()
{
}

void ParticleFilter::motionUpdate(double fw_delta_mm,double t_delta_deg)
{
	double t_delta_rad = t_delta_deg * 3.141592 / 180;

	for(auto &p : m_particles){
		//xy方向
		p.x_mm += fw_delta_mm * cos(p.t_rad);
		p.y_mm += fw_delta_mm * sin(p.t_rad);

		//theta方向
		p.t_rad += t_delta_rad;
	}
}

unsigned int ParticleFilter::getIntRand()
{
	char buf[4];
	m_rand_ifs->read(buf,4);
	return (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
}

void ParticleFilter::pointReset(double x_mm,double y_mm,double t_deg)
{
	int size = m_particles.size();
	for(auto &p : m_particles){
		p = {x_mm,y_mm,t_deg/180*3.141592,1.0/size};
	}
}

double ParticleFilter::getDoubleRand()
{
	return (double)getIntRand() / UINT_MAX;
}

void ParticleFilter::print(ofstream *ofs)
{
	for(auto &p : m_particles){
		*ofs << (int)p.x_mm << " " << (int)p.y_mm << " "
			<< (int)(p.t_rad / 3.141592 * 180) << " " << p.w << endl;
	}
}
