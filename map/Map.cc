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
	print();
	cout << endl;
	printEachRoom();
}

Map::~Map()
{
}

void Map::print(void)
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
					cout << "+---";
				else
					cout << "+   ";
			}
			cout << "+";
		}else{
			for(int j=0;j<m_x_room_num+1;j++){
				if(m_vertical[j+(i/2)*(m_x_room_num+1)])
					cout << "|   ";
				else
					cout << "    ";
			}
		}
		cout << endl;
	}
}

void Map::printEachRoom(void)
{
	int i = 0;
	for(auto r : m_rooms){
		cout << "* ROOM " << i++ << endl;
		r.print();
		cout << endl;
	}
}

// theta should be normalized
bool Map::faceWall(double global_x_mm, double global_y_mm, double theta_rad)
{
	return false;
}
