#include "Room.h"
#include <iostream>
#include <cmath>
using namespace std;

Room::Room(bool n,bool w,bool e,bool s,double x_origin_mm, double y_origin_mm)
{
	m_wall.push_back(n);
	m_wall.push_back(w);
	m_wall.push_back(e);
	m_wall.push_back(s);

	m_x_origin_mm = x_origin_mm;
	m_y_origin_mm = y_origin_mm;
/*
        wall0
+-------------------+
|                   |
|                   |
|                   |
|wall1              |wall2
|                   |
|                   |
|                   |
|                   |
+-------------------+
        wall3     
*/ 
}

Room::~Room()
{
}

void Room::print(void)
{
	cout << "(x,y) = (" << m_x_origin_mm << ", " << m_y_origin_mm << ")" << endl;
	//north
	if(m_wall.at(0))
		cout << "+-------+" << endl;
	else
		cout << "+       +" << endl;

	//west & east
	if(m_wall.at(1)){
		for(int i=0;i<3;i++){
			cout << "|";
			if(m_wall.at(2))
				cout << "       |";

			cout << endl;
		}
	}else{
		for(int i=0;i<3;i++){
			cout << " ";
			if(m_wall.at(2))
				cout << "       |";

			cout << endl;
		}
	}
	//south
	if(m_wall.at(3))
		cout << "+-------+" << endl;
	else
		cout << "+       +" << endl;
}

// theta should be normalized
bool Room::faceWall(double global_x_mm, double global_y_mm, double theta_rad)
{
	double local_x_mm = global_x_mm - m_x_origin_mm;
	double local_y_mm = global_y_mm - m_y_origin_mm;

	double sin_theta = sin(theta_rad);
	double cos_theta = cos(theta_rad);

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

	return false;
}
