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
	ofs << "step " << step << endl;
	drec.print(&ofs);

	while(1){
		string action;
		int val;
		cin >> action >> val;
		if(!cin)
			break;

		if(action == "sleep"){
			usleep(val);
			continue;
		}

		step++;

		if(action == "turn"){
			Actions::turn(val);
			drec.motionUpdate((double)val,0.0);
		}else if(action == "forward"){
			Actions::forward(val);
			drec.motionUpdate(0.0,(double)val);
		}

		ofs << "step " << step << endl;
		drec.print(&ofs);
	}
	
	exit(0);
}
