#include "Actions.h"
#include <fstream>
#include <cmath>
using namespace std;

Actions::Actions(){}
Actions::~Actions(){}

void Actions::inputMotor(int l_hz, int r_hz,int msec)
{
	ofstream ofs("/dev/rtmotor0");
	ofs << l_hz << " " << r_hz << " " << msec << endl;
	ofs.close();
}

void Actions::forward(int mm)
{
	int r_hz = 385;
	int l_hz = 415;
	int msec = (int)floor(400.0*2/(r_hz+l_hz) * 1000*mm/(45*3.141592));

	if(msec < 0)
		inputMotor(-l_hz,-r_hz,-msec);
	else
		inputMotor(l_hz,r_hz,msec);
}

void Actions::turn(int deg)
{
	int hz = 400;
	int adj = 70;
	int msec = (int)floor(400.0/hz * 1000.0*deg/180);

	if(msec < 0)
		inputMotor(hz+adj,-hz-adj,-msec);
	else
		inputMotor(-hz-adj,hz+adj,msec);
}
