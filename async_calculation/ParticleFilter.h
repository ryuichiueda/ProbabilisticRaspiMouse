#ifndef __PF_H_
#define __PF_H_

#include <vector>
#include <fstream>
using namespace std;

class Room;

struct Particle{
	double x_mm, y_mm, t_rad, w;
};

class ParticleFilter
{
public:
	ParticleFilter(int num,ifstream *ifs);
	virtual ~ParticleFilter();

	void motionUpdate(double fw_delta_mm,double t_delta_deg);
	void sensorUpdate(void);

	void pointReset(double x_mm,double y_mm,double t_deg,
			double pos_max_noise_mm,double dir_max_noise_deg);

	void rangeReset(double x_mm_min,double x_mm_max,
			double y_mm_min,double y_mm_max,
			double t_deg_min,double t_deg_max);

	void print(ofstream *ofs);
private:
	ifstream *m_rand_ifs;
	vector<Particle> m_particles;
	int m_step;

	void sensorUpdateNoWallFront(void);

	void resampling(void);

	//noise
	double m_distance_max_noise_ratio;
	double m_direction_max_noise_ratio;

	//utilities
	unsigned int getIntRand();
	double getDoubleRand();

	void normalizeTheta(double *theta_rad);

	Room *m_room;
};

#endif
