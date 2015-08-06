#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <unistd.h>
#include "Actions.h"
#include "DeadReckoning.h"
using namespace std;

int main(int argc, char const* argv[])
{
	ofstream ofs("/tmp/deadrec");
	int step = 0;
	DeadReckoning drec(0.0, 0.0, 90.0);
	drec.print(&ofs);

	while(1){
		string action;
		int val;
		cin >> action >> val;
		if(!cin)
			break;

		step++;

		if(action == "turn"){
			Actions::turn(val);
			drec.motionUpdate((double)val,0.0);
		}else if(action == "forward"){
			Actions::forward(val);
			drec.motionUpdate(0.0,(double)val);
		}

		drec.print(&ofs);
		usleep(100000);
	}
	
	exit(0);
}
