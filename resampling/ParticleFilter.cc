#include <iostream>
#include <limits.h>
#include <cmath>
#include "ParticleFilter.h"
#include "Room.h"
#include "Sensors.h"
using namespace std;

ParticleFilter::ParticleFilter(int num, ifstream *ifs)
{
	Particle p{0.0, 0.0, 0.0, 1.0/num};
	m_particles.insert(m_particles.begin(),num,p);

	m_rand_ifs = ifs;
	m_step = 0;

	m_distance_max_noise_ratio = 0.05; // 5% noise
	m_direction_max_noise_ratio = 0.20; // 20% noise

	m_room = new Room();
}

ParticleFilter::~ParticleFilter()
{
	delete m_room;
}

void ParticleFilter::motionUpdate(double fw_delta_mm,double t_delta_deg)
{
	double t_delta_rad = t_delta_deg * 3.141592 / 180;

	for(auto &p : m_particles){
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

		normalizeTheta(&(p.t_rad));
	}
	m_step++;
}

void ParticleFilter::sensorUpdate(void)
{
	if(Sensors::noWallFront()){
		sensorUpdateNoWallFront();
		resampling();
	}
}

void ParticleFilter::resampling(void) // systematic sampling
{
	vector<Particle> prev;

	double sum_weight = 0.0;
	int num = (int)m_particles.size();
	for(int i = 0;i < num ;i++){
		if(m_particles[i].w < 1.0e-10)
			continue;

		//weight is changed to the accumurated value
		m_particles[i].w += sum_weight;
		sum_weight = m_particles[i].w;
		prev.push_back(m_particles[i]);
	}
	if(prev.size() == 0)
		return;

	double step = sum_weight / num;
	int* choice = new int[num];
	double accum = getDoubleRand() / num;
	int j = 0;
	for(int i=0;i<num;i++){
		if(prev[j].w <= accum)
			j++;

		if(j == num)
			j--;

		accum += step;
		choice[i] = j;
	}

	for(int i=0;i<num;i++){
		int j = choice[i];
		m_particles[i] = prev[j];
		m_particles[i].w = 1.0/num;
	}

	delete [] choice;
}

void ParticleFilter::sensorUpdateNoWallFront(void)
{
	for(auto &p : m_particles){
		if(m_room->faceWall(p.x_mm,p.y_mm,p.t_rad))
			p.w *= 0.00001;
	}
}

void ParticleFilter::pointReset(double x_mm,double y_mm,double t_deg,
				double pos_max_noise_mm,double dir_max_noise_deg)
{
	for(auto &p : m_particles){
		double dir_noise = dir_max_noise_deg *(2*getDoubleRand() - 1.0);

		double pos_noise = pos_max_noise_mm * getDoubleRand();
		double pos_noise_angle = getDoubleRand() * 2 * 3.141592;
		double x_noise = pos_noise * cos(pos_noise_angle);
		double y_noise = pos_noise * sin(pos_noise_angle);

		p = {x_mm+x_noise, y_mm+y_noise, (t_deg + dir_noise)/180*3.141592};

		normalizeTheta(&(p.t_rad));
	}
}

void ParticleFilter::rangeReset(double x_mm_min,double x_mm_max,
				double y_mm_min,double y_mm_max,
				double t_deg_min,double t_deg_max)
{
	double x_mm_delta = x_mm_max - x_mm_min;
	double y_mm_delta = y_mm_max - y_mm_min;
	double t_deg_delta = t_deg_max - t_deg_min;

	for(auto &p : m_particles){
		p.x_mm = x_mm_min + x_mm_delta * getDoubleRand();
		p.y_mm = y_mm_min + y_mm_delta * getDoubleRand();
		p.t_rad = (t_deg_min + t_deg_delta * getDoubleRand())/180*3.141592;

		normalizeTheta(&(p.t_rad));
	}
}

// from 0 to UINT_MAX
unsigned int ParticleFilter::getIntRand()
{
	char buf[4];
	m_rand_ifs->read(buf,4);
	return (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
}

// from 0.0 to 1.0
double ParticleFilter::getDoubleRand()
{
	return (double)getIntRand() / UINT_MAX;
}

void ParticleFilter::print(ofstream *ofs)
{
	for(auto &p : m_particles){
		*ofs << m_step << " " << (int)p.x_mm << " " << (int)p.y_mm << " "
			<< (int)(p.t_rad / 3.141592 * 180) << " " << p.w << endl;
	}
}

void ParticleFilter::normalizeTheta(double *theta_rad)
{
	while(*theta_rad >= 2*3.141592)
		*theta_rad -= 2*3.141592;
	while(*theta_rad < 0.0)
		*theta_rad += 2*3.141592;
}
