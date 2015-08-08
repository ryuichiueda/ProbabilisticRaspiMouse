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
	bool no_wall = true;
	int val = 5;

	int counter = 0;

	while(1){
		if(no_wall){
			thread async_motion(motion,"forward",30);
			no_wall = Sensors::noWallFront();
			async_motion.join();


			if(!no_wall){
				if(counter%5 == 0)
					val *= -1;
				counter++;
			}
		}else{
			thread async_motion(motion,"turn",val);
			no_wall = Sensors::noWallFront();
			async_motion.join();
		}
	}
	exit(0);
}
