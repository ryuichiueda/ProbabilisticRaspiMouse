#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
#include "ParticleFilter.h"
using namespace std;

int main(int argc, char const* argv[])
{
	ofstream ofs_input("/tmp/deadrec");
	ofstream ofs_particles("/tmp/particles");

	ifstream urandom("/dev/urandom");
	ParticleFilter pf(10,&urandom);

	//初期姿勢
	double x = 0;
	double y = 0;
	double deg = 90.0;
	ofs_input << 0 << " " << x << " " << y << " " << deg << endl;
	pf.pointReset(x,y,deg);
	ofs_particles << "step 0" << endl;
	pf.print(&ofs_particles);

	//1回目の指令
	Actions::turn(90);
	deg += 90.0;
	ofs_input << 1 << " " << x << " " << y << " " << deg << endl;
	pf.motionUpdate(0.0,90.0);
	ofs_particles << "step 1" << endl;
	pf.print(&ofs_particles);

	//2回目の指令
	Actions::forward(300);
	x += 300 * cos(deg * 3.141592 / 180);
	y += 300 * sin(deg * 3.141592 / 180);
	ofs_input << 2 << " " << x << " " << y << " " << deg << endl;
	pf.motionUpdate(300.0,0.0);
	ofs_particles << "step 2" << endl;
	pf.print(&ofs_particles);

	exit(0);
}
