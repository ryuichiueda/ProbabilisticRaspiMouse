#include <iostream>
#include <limits.h>
#include <cmath>
#include "ParticleFilter.h"
#include "Map.h"
#include "Room.h"
#include "SensorRange.h"
using namespace std;

const double ParticleFilter::m_near_zero = 1e-10;
const double ParticleFilter::m_sensor_reset_th = 0.05;
const double ParticleFilter::m_expansion_reset_th = 0.20;
const double ParticleFilter::m_reset_th = 0.20;

ParticleFilter::ParticleFilter(int num, ifstream *ifs,string mapfile)
{
	Particle p{0.0, 0.0, 0.0, 1.0/num};
	m_particles.insert(m_particles.begin(),num,p);

	m_rand_ifs = ifs;
	m_step = 0;

	m_distance_max_noise_ratio = 0.10; // 5% noise
	m_direction_max_noise_ratio = 0.10; // 10% noise

	m_map = new Map(mapfile);
}

ParticleFilter::~ParticleFilter()
{
	delete m_map;
}

void ParticleFilter::motionUpdate(double fw_delta_mm,double side_delta_mm,double t_delta_deg)
{
	double t_delta_rad = t_delta_deg * 3.141592 / 180;

	for(auto &p : m_particles){
		double t_shift = 0.0;//change of direction due to foward action

		if(fw_delta_mm > 0.1){ //xy方向
			double pos_noise = m_distance_max_noise_ratio * getDoubleRand();
			double pos_noise_angle = getDoubleRand() * 2 * 3.141592;
			double x_noise = pos_noise * cos(pos_noise_angle);
			double y_noise = pos_noise * sin(pos_noise_angle);
	
			double nonoise_x = p.x_mm + fw_delta_mm * cos(p.t_rad);
			double nonoise_y = p.y_mm + fw_delta_mm * sin(p.t_rad);
	
			double after_x = p.x_mm + fw_delta_mm * (cos(p.t_rad) + x_noise);
			double after_y = p.y_mm + fw_delta_mm * (sin(p.t_rad) + y_noise);
	
			double x1 = after_x - p.x_mm;
			double y1 = after_y - p.y_mm;
			double x2 = nonoise_x - p.x_mm;
			double y2 = nonoise_y - p.y_mm;
			t_shift = atan2(x2*y1 - x1*y2,x1*x2 + y1*y2);

			bool col[4];
			m_map->collision(p.x_mm,p.y_mm,after_x,after_y,col);

			if(col[1] || col[2]){
				p.t_rad += (getDoubleRand() - 0.5)*3.141592;	
			}else{
				p.x_mm = after_x;
			}
	
			if(col[0] || col[3]){
				p.t_rad += (getDoubleRand() - 0.5)*3.141592;	
			}else{
				p.y_mm = after_y;
			}
		}

		//theta方向
		double ratio = m_direction_max_noise_ratio * (2*getDoubleRand()-1.0);
		p.t_rad += t_delta_rad * (1.0 + ratio) + t_shift;
		normalizeTheta(&(p.t_rad));
	}
	m_step++;
}

void ParticleFilter::sensorUpdate(void)
{
	sensorUpdateMazeInOut();

	if(SensorRange::noWallFront()){
		cerr << "no wall" << endl;
		sensorUpdateNoWallFront();
	}
	if(SensorRange::wallFront()){
		cerr << "wall" << endl;
		sensorUpdateWallFront();
	}

	if(!reset())
		resampling();
}

void ParticleFilter::resampling(void) // systematic sampling
{
	vector<Particle> prev;

	double sum_weight = 0.0;
	for(auto &p : m_particles){
		p.w += sum_weight;
		sum_weight = p.w;
		prev.push_back(p);
	}

	int num = (int)m_particles.size();
	vector<int> choice(num);

	double accum = getDoubleRand() / num;
	double step = sum_weight / num;

	int j = 0;
	for(auto &c : choice){
		while(prev[j].w <= accum && j < num - 1)
			j++;

		c = j;
		accum += step;
	}

	for(int i=0;i<num;i++){
		m_particles[i] = prev[choice[i]];
		m_particles[i].w = 1.0/num;
	}
}

void ParticleFilter::sensorUpdateWallFront(void)
{
	for(auto &p : m_particles){
		if(!m_map->faceWall(p.x_mm,p.y_mm,p.t_rad)){
			p.w *= m_near_zero;
		}
	}
}

void ParticleFilter::sensorUpdateNoWallFront(void)
{
	for(auto &p : m_particles){
		if(m_map->faceWall(p.x_mm,p.y_mm,p.t_rad)){
			p.w *= m_near_zero;
		}
	}
}

void ParticleFilter::pointReset(double x_mm,double y_mm,double t_deg,
				double pos_max_noise_mm,double dir_max_noise_deg)
{
	int size = (int)m_particles.size();

	for(auto &p : m_particles){
		double dir_noise = dir_max_noise_deg * getGaussRand();

		double pos_noise = pos_max_noise_mm * getGaussRand();
		double pos_noise_angle = getDoubleRand() * 2 * 3.141592;
		double x_noise = pos_noise * cos(pos_noise_angle);
		double y_noise = pos_noise * sin(pos_noise_angle);

		p = {x_mm+x_noise, y_mm+y_noise, (t_deg + dir_noise)/180*3.141592,1.0/size};

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

	int size = (int)m_particles.size();

	for(auto &p : m_particles){
		p.x_mm = -100000.0;
		p.y_mm = -100000.0;

		while(!m_map->inTheMaze(p.x_mm,p.y_mm)){
			p.x_mm = x_mm_min + x_mm_delta * getDoubleRand();
			p.y_mm = y_mm_min + y_mm_delta * getDoubleRand();
		}

		p.t_rad = (t_deg_min + t_deg_delta * getDoubleRand())/180*3.141592;
		p.w = 1.0/size;
		normalizeTheta(&(p.t_rad));
	}
}

void ParticleFilter::randomReset(void)
{
	int size = (int)m_particles.size();

	for(auto &p : m_particles){
		p.x_mm = -100000.0;
		p.y_mm = -100000.0;

		while(!m_map->inTheMaze(p.x_mm,p.y_mm)){
			p.x_mm = m_map->getWidth() * getDoubleRand();
			p.y_mm = m_map->getHeight() * getDoubleRand();
		}

		p.t_rad = (360 * getDoubleRand())/180*3.141592;
		p.w = 1.0/size;
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

double ParticleFilter::getGaussRand()
{
	double sum = 0.0;
	for(int i=0;i<12;i++)
		sum += (double)getDoubleRand();

	return sum - 6.0;
}

void ParticleFilter::print(ofstream *ofs)
{
	for(auto &p : m_particles){
		*ofs << m_step << " " << time(NULL)
			<< " " << (int)p.x_mm << " " << (int)p.y_mm << " "
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

Particle ParticleFilter::getAverage(void)
{
	Particle ans = {0.0,0.0,0.0};
	double wsum = 0.0;
	double t_rad_avg = 0.0;
	double t_rad_avg_shift = 0.0;
	for(auto p : m_particles){
		wsum += p.w;
		ans.x_mm += p.x_mm * p.w;
		ans.y_mm += p.y_mm * p.w;

		//the border between 0 and 2pi should be considered
		//on theta axis
		normalizeTheta(&p.t_rad);
		t_rad_avg += p.t_rad * p.w;
		t_rad_avg_shift += (p.t_rad + 3.141592) * p.w; 
	}
	ans.x_mm /= wsum;
	ans.y_mm /= wsum;
	t_rad_avg /= wsum;
	t_rad_avg_shift /= wsum;

	double t_rad_error = 0.0;
	double t_rad_error_shift = 0.0;
	for(auto p : m_particles){
		t_rad_error = (p.t_rad - t_rad_avg)*(p.t_rad - t_rad_avg)*p.w;
		t_rad_error_shift = (p.t_rad + 3.141592 - t_rad_avg_shift)
			*(p.t_rad + 3.141592 - t_rad_avg_shift)*p.w;
	}

	if(t_rad_error <= t_rad_error_shift)
		ans.t_rad = t_rad_avg;
	else{
		t_rad_avg_shift -= 3.141592;
		normalizeTheta(&t_rad_avg_shift);
		ans.t_rad = t_rad_avg_shift;
	}

	return ans;
}

void ParticleFilter::sensorUpdateMazeInOut(void)
{
	for(auto &p : m_particles)
		if(!m_map->inTheMaze(p.x_mm,p.y_mm))
			p.w *= m_near_zero;
}

double ParticleFilter::sumOfWeights(void)
{
	double sum = 0.0;
	for(auto p : m_particles)
		sum += p.w;

	return sum;
}

bool ParticleFilter::reset(void)
{
	static int reset_times = 0;
	if(sumOfWeights() < m_reset_th){
		if(reset_times > 0){
			cerr << "sr" << endl;
			sensorReset();
		}else{
			cerr << "er" << endl;
			expansionReset();
		}

		reset_times++;
		cerr << reset_times << endl;
		return true;
	}else{
		reset_times = 0;
	}
	return false;

//	randomReset();
}

//inefficient implementation
void ParticleFilter::sensorReset(void)
{
	int size = (int)m_particles.size();

	for(auto &p : m_particles){
		p.x_mm = -100000.0;
		p.y_mm = -100000.0;

		while(!m_map->inTheMaze(p.x_mm,p.y_mm)){
			p.x_mm = m_map->getWidth() * getDoubleRand();
			p.y_mm = m_map->getHeight() * getDoubleRand();
			p.t_rad = (360 * getDoubleRand())/180*3.141592;

			if(SensorRange::noWallFront()){
				if(m_map->faceWall(p.x_mm,p.y_mm,p.t_rad))
					p.x_mm = -100000.0;
			}
			if(SensorRange::wallFront()){
				if(!m_map->faceWall(p.x_mm,p.y_mm,p.t_rad))
					p.x_mm = -100000.0;
			}
		}
		p.w = 1.0/size;
		normalizeTheta(&(p.t_rad));
	}
}

void ParticleFilter::expansionReset(void)
{
	const double max_exp_length = 50.0;
	const double max_exp_angle = 10.0;

	int size = (int)m_particles.size();

	for(auto &p : m_particles){
		double exp_length = max_exp_length * getDoubleRand();
		double dir_rad = (360 * getDoubleRand())/180*3.141592;
		p.x_mm += exp_length * cos(dir_rad);
		p.y_mm += exp_length * sin(dir_rad);
		p.t_rad += (2*max_exp_angle*getDoubleRand() - max_exp_angle)/180*3.141592;

		p.w = 1.0/size;
		normalizeTheta(&(p.t_rad));
	}

}
