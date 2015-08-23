#ifndef SENSOR_H__
#define SENSOR_H__

#include <fstream>
using namespace std;

class Sensor {
public:
	Sensor(){}
	virtual ~Sensor(){}
	virtual void update(void) = 0;
};

#endif
