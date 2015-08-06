#ifndef __DRMC_H_
#define __DRMC_H_

#include <vector>
#include <fstream>
using namespace std;

struct Pose{
	double x_mm, y_mm, t_rad;
};

class DeadRecMonteCarlo
{
public:
	DeadRecMonteCarlo(int num,ifstream *ifs);
	virtual ~DeadRecMonteCarlo();

	void motionUpdate(double fw_delta_mm,double t_delta_deg);
	void pointReset(double x_mm,double y_mm,double t_deg,
			double pos_max_noise_mm,double dir_max_noise_deg);

	void print(ofstream *ofs);
private:
	ifstream *m_rand_ifs;
	vector<Pose> m_poses;
	int m_step;

	//noise
	double m_distance_max_noise_ratio;
	double m_direction_max_noise_ratio;

	//utilities
	unsigned int getIntRand();
	double getDoubleRand();
};

#endif
