#ifndef SENSOR_GYRO_H__
#define SENSOR_GYRO_H__

#include "Sensor.h"
#include <fstream>
using namespace std;

struct GyroData{
	int x; int y; int z;
};

class SensorGyro {
public:
	SensorGyro(){};
	~SensorGyro(){};

	static void print(ofstream *ofs);

	static void init(void);
	static void update(void);

	static double getDeltaDeg(void){return m_cur_deg - m_prev_deg;}
private:
	static double m_prev_deg,m_cur_deg;
	static GyroData m_zero;
	//ifstream m_gyro_file;

	static bool read(GyroData *ans);
	static bool m_initialized;
};

#endif
