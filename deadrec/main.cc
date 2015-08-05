#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
using namespace std;


int main(int argc, char const* argv[])
{
	ofstream ofs("/tmp/deadrec");

	double x = 0;
	double y = 0;
	double deg = 90.0;

	ofs << x << " " << y << " " << deg << endl;

	Actions::turn(90);
	deg += 90.0;

	Actions::forward(300);
	x += 300 * cos(deg * 3.141592 / 180);
	y += 300 * sin(deg * 3.141592 / 180);

	ofs << x << " " << y << " " << deg << endl;

	return 0;
}
