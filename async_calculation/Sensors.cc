#include "Sensors.h"
using namespace std;

Sensors::Sensors(){}
Sensors::~Sensors(){}

bool Sensors::noWallFront(void)
{
	SensorValues vs = readRangeSensors();
	return (vs.left_front <= 500 && vs.right_front <= 500);
}

void Sensors::print(ofstream *ofs)
{
	SensorValues ans = readRangeSensors();
	*ofs << ans.left_front << " " << ans.left_side << " "
		<< ans.right_side << " " << ans.right_front << endl;
}

SensorValues Sensors::readRangeSensors(void)
{
	SensorValues ans;

	ifstream ifs("/dev/rtlightsensor0");
	ifs >> ans.right_front >> ans.right_side >> ans.left_side >> ans.left_front;
	ifs.close();

	return ans;
}

