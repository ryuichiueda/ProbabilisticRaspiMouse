#ifndef __PF_H_
#define __PF_H_

#include <vector>
#include <fstream>
using namespace std;

struct Particle{
	double x_mm;
	double y_mm;
	double t_rad;
	double w;
};

class ParticleFilter
{
public:
	ParticleFilter(int num,ifstream *ifs);
	virtual ~ParticleFilter();

	void motionUpdate(double fw_delta_mm,double t_delta_deg);

	void print(ofstream *ofs);

	void pointReset(double x_mm,double y_mm,double t_deg);
private:
	ifstream *m_rand_ifs;
	vector<Particle> m_particles;

	unsigned int getIntRand();
	double getDoubleRand();
};

#endif
