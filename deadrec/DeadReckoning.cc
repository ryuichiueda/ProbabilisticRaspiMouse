#include "DeadReckoning.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits.h>
#include <cmath>
using namespace std;

DeadReckoning::DeadReckoning(double init_x_mm, double init_y_mm, double init_t_deg)
{
	m_pose = {init_x_mm,init_y_mm,init_t_deg/180*3.141592};
	m_step = 0;
}

DeadReckoning::~DeadReckoning()
{
}

void DeadReckoning::motionUpdate(double t_delta_deg,double fw_delta_mm)
{
	double t_delta_rad = t_delta_deg * 3.141592 / 180;

	m_pose.x_mm += fw_delta_mm * cos(m_pose.t_rad + t_delta_rad);
	m_pose.y_mm += fw_delta_mm * sin(m_pose.t_rad + t_delta_rad);
	m_pose.t_rad += t_delta_rad;

	m_step++;
}

void DeadReckoning::print(ofstream *ofs)
{
	*ofs << "step" << m_step << " "
		<< (int)m_pose.x_mm << " " << (int)m_pose.y_mm << " "
		<< (int)(m_pose.t_rad / 3.141592 * 180) << endl;
}
