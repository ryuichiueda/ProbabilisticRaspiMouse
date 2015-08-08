#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
#include "Sensors.h"
#include <unistd.h>
#include <thread>
using namespace std;

void motion(string action, int val)
{
	if(action == "turn"){
		Actions::turn(val);
	}else if(action == "forward"){
		Actions::forward(val);
	}
}

int main(int argc, char const* argv[])
{
	int seq = 0;
	bool right = true;
	bool no_wall = true;

	while(1){
		if(seq < 3 && no_wall){
			thread async_motion(motion,"forward",30);
			no_wall = Sensors::noWallFront();

			if(rand()%2)	right = true;
			else		right = false;
			seq++;

			async_motion.join();
		}else{
			int val = 5;
			if(right)
				val *= -1;

			thread async_motion(motion,"turn",val);
			no_wall = Sensors::noWallFront();

			seq = 0;

			async_motion.join();
		}
	}
	exit(0);
}
