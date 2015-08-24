#ifndef __MAP_H_
#define __MAP_H_

#include <vector>
#include <string>
using namespace std;

class Room;

class Map
{
public:
	Map(string mapfile);
	virtual ~Map();

	bool faceWall(double global_x_mm, double global_y_mm, double theta_rad);
	bool collision(double org_global_x_mm, double org_global_y_mm,
			double global_x_mm, double global_y_mm,bool *detail);
	bool inTheMap(double x_mm, double y_mm);

	void print(ofstream *ofs);
	void printEachRoom(ofstream *ofs);
	void printImage(ofstream *ofs);

private:
	vector<bool> m_horizontal;
	vector<bool> m_vertical;

	vector<Room> m_rooms;

	int m_x_room_num;
	int m_y_room_num;

	int m_x_width_mm;
	int m_y_width_mm;

	Room *posToRoom(double global_x_mm,double global_y_mm);
};

#endif
