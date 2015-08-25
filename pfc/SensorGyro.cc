#include "SensorGyro.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
using namespace std;

double SensorGyro::m_deg = 0.0;
double SensorGyro::m_prev = 0.0;


void SensorGyro::print(ofstream *ofs)
{
	*ofs << m_deg << " " << m_prev << endl;
}

void SensorGyro::update(void)
{
	ifstream ifs("/dev/ttyACM0");
	if(!ifs){
		int restart;
		cerr << "no gyro" << endl;
		cin >> restart;
	}

	m_prev = m_deg;
	double tmp;
	ifs >> tmp >> m_deg;
	ifs.close();
}

