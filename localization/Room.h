#ifndef __ROOM_H_
#define __ROOM_H_

#include <vector>
#include <fstream>
using namespace std;


class Room
{
public:
	Room(bool n,bool w,bool e,bool s,double x_origin_mm, double y_origin_mm);
	virtual ~Room();

	bool faceWall(double global_x_mm, double global_y_mm, double theta_rad);
	bool collisionNorth(double org_global_y_mm,double global_y_mm);
	bool collisionWest(double org_global_x_mm,double global_x_mm);
	bool collisionEast(double org_global_x_mm,double global_x_mm);
	bool collisionSouth(double org_global_y_mm,double global_y_mm);

	bool isClosed(void);

	void print(ofstream *ofs);
	void printImage(ofstream *ofs,bool *image);
private:
	//vector<bool> m_pin;
	vector<bool> m_wall;

	double m_x_origin_mm;
	double m_y_origin_mm;
};

/*

pin0    wall0      pin1
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
pin2  wall3      pin3
*/

#endif
