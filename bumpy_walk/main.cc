#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
#include "Sensors.h"
#include <unistd.h>
using namespace std;

int main(int argc, char const* argv[])
{
	int seq = 0;
	bool right = true;
	while(1){
		if(seq < 3 && Sensors::noWallFront()){
			Actions::forward(180);
			if(rand()%2)
				right = true;
			else
				right = false;
			seq++;
		}else{
			int val = 30;
			if(right)
				val *= -1;
			Actions::turn(val);
			seq = 0;
		}
		usleep(100000);
	}
	exit(0);
}
