#ifndef SENSORS_H__
#define SENSORS_H__

#include <fstream>
using namespace std;

struct SensorValues {
	int left_front,left_side,right_side,right_front;
};

class Sensors {
public:
	Sensors();
	~Sensors();

	static bool noWallFront(void);
	static void print(ofstream *ofs);
private:
	static SensorValues readRangeSensors(void);
};

#endif
