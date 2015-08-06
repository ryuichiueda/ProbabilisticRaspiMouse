#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
#include "DeadRecMonteCarlo.h"
#include <unistd.h>
using namespace std;

int main(int argc, char const* argv[])
{
	ofstream ofs("/tmp/particles");

	ifstream urandom("/dev/urandom");
	DeadRecMonteCarlo pf(10,&urandom);
	pf.pointReset(0.0,0.0,90.0,5.0,5.0); //consideration of 5mm, 5deg error on placement
	pf.print(&ofs);

	while(1){
		string action;
		int val;
		cin >> action >> val;
		if(!cin)
			break;

		if(action == "turn"){
			Actions::turn(val);
			pf.motionUpdate(0.0,(double)val);
		}else if(action == "forward"){
			Actions::forward(val);
			pf.motionUpdate((double)val,0.0);
		}

		pf.print(&ofs);
		usleep(100000);
	}
	
	exit(0);
}
