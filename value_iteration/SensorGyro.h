#ifndef SENSOR_GYRO_H__
#define SENSOR_GYRO_H__

#include "Sensor.h"
#include <fstream>
using namespace std;

class SensorGyro {
public:
	SensorGyro(){};
	virtual ~SensorGyro(){};

	static void print(ofstream *ofs);
	static void update(void);

	static double getDeltaDeg(void){return m_deg - m_prev;}
private:
	static double m_deg,m_prev;

	void read(void);
};

#endif
