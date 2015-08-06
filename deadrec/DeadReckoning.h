#ifndef __DREC_H_
#define __DREC_H_

#include <fstream>
using namespace std;

struct Pose{
	double x_mm, y_mm, t_rad;
};

class DeadReckoning
{
public:
	DeadReckoning(double init_x_mm, double init_y_mm, double init_t_deg);
	~DeadReckoning();

	void motionUpdate(double t_delta_deg, double fw_delta_mm);
	void print(ofstream *ofs);
private:
	Pose m_pose;
};

#endif
