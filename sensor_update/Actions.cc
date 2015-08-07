#include "Actions.h"
#include <fstream>
#include <cmath>
using namespace std;

Actions::Actions(){}
Actions::~Actions(){}

void Actions::inputMotor(int l_hz, int r_hz,int msec)
{
	ofstream ofs("/dev/rtmotor0");
	ofs << r_hz << " " << l_hz << " " << msec << endl;
	ofs.close();
}

void Actions::forward(int mm)
{
	int hz = 400;
	int msec = (int)floor(400.0/hz * 1000*mm/(45*3.141592));

	if(msec < 0)
		inputMotor(-hz,-hz,-msec);
	else
		inputMotor(hz,hz,msec);
}

void Actions::turn(int deg)
{
	int hz = 400;
	int msec = (int)floor(400.0/hz * 1000.0*deg/180);

	if(msec < 0)
		inputMotor(-hz,hz,-msec);
	else
		inputMotor(hz,-hz,msec);
}
