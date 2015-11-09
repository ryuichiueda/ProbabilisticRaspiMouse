#ifndef SENSOR_RANGE_H__
#define SENSOR_RANGE_H__

#include "Sensor.h"

class SensorRange {
public:
	SensorRange(){}
	~SensorRange(){}

	static bool noWallFront(void);
	static bool wallFront(void);
	static void print(ofstream *ofs);

	static void update(void);

	static int getLF(void){return m_left_front;}
	static int getRF(void){return m_right_front;}
	static int getLS(void){return m_left_side;}
	static int getRS(void){return m_left_front;}
private:
	static int m_left_front,m_left_side,m_right_side,m_right_front;
};


#endif
