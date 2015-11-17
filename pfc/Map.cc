#include "Map.h"
#include "Room.h"
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

Map::Map(string mapfile)
{
	m_x_room_num = 4;
	m_y_room_num = 4;

	m_x_width_mm = m_x_room_num * 180.0;
	m_y_width_mm = m_y_room_num * 180.0;

	ifstream ifs(mapfile);
	int flgs = 0;

	for(int i=0;i<m_y_room_num*2+1;i++){
		if(i%2==0){//read horizontal walls
			for(int j=0;j<m_x_room_num;j++){
				ifs >> flgs;
				m_horizontal.push_back(flgs==1);
			}
		}else{//read vertical walls
			for(int j=0;j<m_x_room_num+1;j++){
				ifs >> flgs;
				m_vertical.push_back(flgs==1);
			}
		}
	}
	// crate each room from top left to bottom right
	for(int y=0;y<m_y_room_num;y++){
		for(int x=0;x<m_x_room_num;x++){
			bool north = m_horizontal.at(x + y*m_x_room_num);
			bool south = m_horizontal.at(x + (y+1)*m_x_room_num);
			bool west = m_vertical.at(x + y*(m_x_room_num+1));
			bool east = m_vertical.at(x+1 + y*(m_x_room_num+1));

			double shift_x = 180*x + 90;
			double shift_y = 180*(m_y_room_num-y) - 90;

			m_rooms.push_back(Room(north,west,east,south,shift_x,shift_y));
		}
	}

	ofstream maplog("./__map");
	print(&maplog);
	printEachRoom(&maplog);
}

Map::~Map()
{
}

void Map::print(ofstream *ofs)
{
/* draw a map form raw data
+---+---+---+---+
|       |       |   
+   +   +   +   +
|       |       |   
+   +   +   +   +
|       |       |   
+---+   +---+---+
|   |           |   
+---+---+---+---+
*/
	for(int i=0;i<m_y_room_num*2+1;i++){
		if(i%2==0){
			for(int j=0;j<m_x_room_num;j++){
				if(m_horizontal[j+(i/2)*m_x_room_num])
					*ofs << "+---";
				else
					*ofs << "+   ";
			}
			*ofs << "+";
		}else{
			for(int j=0;j<m_x_room_num+1;j++){
				if(m_vertical[j+(i/2)*(m_x_room_num+1)])
					*ofs << "|   ";
				else
					*ofs << "    ";
			}
		}
		*ofs << endl;
	}
}

void Map::printImage(ofstream *ofs)
{
	bool image[360*360];
	for(auto r : m_rooms){
		r.printImage(ofs,image);
	}
}

void Map::printEachRoom(ofstream *ofs)
{
	int i = 0;
	for(auto r : m_rooms){
		*ofs << "* ROOM " << i++ << endl;
		r.print(ofs);
		*ofs << endl;
	}
}

Room *Map::posToRoom(double global_x_mm,double global_y_mm)
{
	int index_x = (int)floor(global_x_mm / 180);
	int index_y = (int)floor((180*4 - global_y_mm) / 180);
	if(index_x < 0 || index_x >= m_x_room_num)
		return NULL;
	if(index_y < 0 || index_y >= m_y_room_num)
		return NULL;

	return &(m_rooms.at(index_x + index_y*m_x_room_num));
}

bool Map::faceWall(double global_x_mm, double global_y_mm, double theta_rad)
{
	Room *rm = posToRoom(global_x_mm,global_y_mm);
	if(rm == NULL)
		return false;

	return rm->faceWall(global_x_mm,global_y_mm,theta_rad);
}

bool Map::collision(double org_global_x_mm, double org_global_y_mm,
			double global_x_mm, double global_y_mm,bool *detail)
{
	Room *rm = posToRoom(global_x_mm,global_y_mm);
	if(rm == NULL)
		return true;

	detail[0] = rm->collisionNorth(org_global_y_mm,global_y_mm);
	detail[1] = rm->collisionWest(org_global_x_mm,global_x_mm);
	detail[2] = rm->collisionEast(org_global_x_mm,global_x_mm);
	detail[3] = rm->collisionSouth(org_global_y_mm,global_y_mm);

	return detail[0] || detail[1] || detail[2] || detail[3];
}

bool Map::inTheMaze(double x_mm, double y_mm)
{
/*
	if(x_mm <= 0.0)			return false;
	if(x_mm >= m_x_width_mm)	return false;
	if(y_mm <= 0.0)			return false;
	if(y_mm >= m_y_width_mm)	return false;
*/

	Room *rm = posToRoom(x_mm,y_mm);
	if(rm == NULL)
		return false;

	return (! rm->isClosed() );
}
