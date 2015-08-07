#ifndef __ROOM_H_
#define __ROOM_H_

#include <vector>
//#include <fstream>
using namespace std;


class Room
{
public:
	Room();
	virtual ~Room();

	bool faceWall(double global_x_mm, double global_y_mm, double theta_rad);
private:
	//vector<bool> m_pin;
	vector<bool> m_wall;

	double m_x_origin;
	double m_y_origin;
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
