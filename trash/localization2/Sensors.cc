#include "Sensors.h"
using namespace std;

SensorValues Sensors::m_values = {0,0,0,0};

bool Sensors::noWallFront(void)
{
	return m_values.left_front <= 500 && m_values.right_front <= 500;
}

bool Sensors::wallFront(void)
{
	return m_values.left_front >= 500 && m_values.right_front >= 500
		&& m_values.left_side >= 500 && m_values.right_side >= 500;
}

void Sensors::print(ofstream *ofs)
{
	*ofs << m_values.left_front << " " << m_values.left_side << " "
		<< m_values.right_side << " " << m_values.right_front << endl;
}

void Sensors::update(void)
{
	ifstream ifs("/dev/rtlightsensor0");
	ifs >> m_values.right_front >> m_values.right_side
		>> m_values.left_side >> m_values.left_front;
	ifs.close();
}

