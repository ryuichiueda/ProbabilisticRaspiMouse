#ifndef __PF_GYRO_H_
#define __PF_GYRO_H_

#include "ParticleFilter.h"
using namespace std;

class ParticleFilterGyro : public ParticleFilter
{
public:
	ParticleFilterGyro(int num,ifstream *ifs,string mapfile);
	virtual ~ParticleFilterGyro();

	virtual void motionUpdate(double fw_delta_mm,double side_delta_mm,double t_delta_deg);
private:
};

#endif
