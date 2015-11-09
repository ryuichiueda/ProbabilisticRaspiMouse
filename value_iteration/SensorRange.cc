#include "SensorRange.h"
using namespace std;

int SensorRange::m_left_front = 0;
int SensorRange::m_left_side = 0;
int SensorRange::m_right_side = 0;
int SensorRange::m_right_front = 0;

bool SensorRange::noWallFront(void)
{
	return m_left_front <= 500 && m_right_front <= 500;
}

bool SensorRange::wallFront(void)
{
	return m_left_front >= 500 && m_right_front >= 500
		&& m_left_side >= 500 && m_right_side >= 500;
}

void SensorRange::print(ofstream *ofs)
{
	*ofs << m_left_front << " " << m_left_side << " "
		<< m_right_side << " " << m_right_front << endl;
}

void SensorRange::update(void)
{
	ifstream ifs("/dev/rtlightsensor0");
	ifs >> m_right_front >> m_right_side
		>> m_left_side >> m_left_front;
	ifs.close();
}

