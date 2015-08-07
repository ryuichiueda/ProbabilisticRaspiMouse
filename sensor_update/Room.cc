#include "Room.h"
#include <iostream>
#include <cmath>
using namespace std;

Room::Room()
{
/*
	m_pin.push_back(false);
	m_pin.push_back(false);
	m_pin.push_back(true);
	m_pin.push_back(true);
*/

	m_wall.push_back(false);
	m_wall.push_back(true);
	m_wall.push_back(true);
	m_wall.push_back(true);

	m_x_origin = 90.0;
	m_y_origin = 90.0;
/*

|                   |
|                   |
|                   |
|wall1              |wall2
|                   |
|                   |
|                   |
|                   |
+-------------------+
pin2  wall3      pin3
*/ 
}

Room::~Room()
{
}

// theta should be normalized
bool Room::faceWall(double global_x_mm, double global_y_mm, double theta_rad)
{
	double local_x_mm = global_x_mm - m_x_origin;
	double local_y_mm = global_y_mm - m_y_origin;

	double sin_theta = sin(theta_rad);
	double cos_theta = cos(theta_rad);

	cerr << "pos" << " " << local_x_mm << " " << local_y_mm << " " << theta_rad / 3.141592 * 180.0 << endl;

	//top
	if(m_wall.at(0) && sin_theta > 0.0001){
		double t = (84.0 - local_y_mm)/sin_theta;
		double x = local_x_mm + t * cos_theta;
		if(fabs(x) <= 84.0)
			return true;
	}
	//left
	if(m_wall.at(1) && cos_theta < -0.0001){
		double t = (- 84.0 - local_x_mm)/cos_theta;
		double y = local_y_mm + t * sin_theta;
		if(fabs(y) <= 84.0)
			return true;
	}
	//right
	if(m_wall.at(2) && cos_theta > 0.0001){
		double t = (84.0 - local_x_mm)/cos_theta;
		double y = local_y_mm + t * sin_theta;
		if(fabs(y) <= 84.0)
			return true;
	}
	//bottom
	if(m_wall.at(3) && sin_theta < -0.0001){
		double t = (-84.0 - local_y_mm)/sin_theta;
		double x = local_x_mm + t * cos_theta;
		if(fabs(x) <= 84.0)
			return true;
	}

	cerr << "nowall" << " " << local_x_mm << " " << local_y_mm << " " << theta_rad / 3.141592 * 180.0 << endl;

	return false;
}
