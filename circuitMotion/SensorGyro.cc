#include "SensorGyro.h"
#include <iostream>
#include <unistd.h>
using namespace std;

double SensorGyro::m_prev_deg = 0.0;
double SensorGyro::m_cur_deg = 0.0;
bool SensorGyro::m_initialized = false;
GyroData SensorGyro::m_zero = {0,0,0};

void SensorGyro::print(ofstream *ofs)
{
	*ofs << m_cur_deg << " " << m_prev_deg << endl;
}

bool SensorGyro::read(GyroData *ans)
{
	FILE *p = fopen("/dev/ttyACM0","r");
	unsigned char data[29];
	fgets((char *)data,28,p);
	fclose(p);
/*
	ifstream m_gylo_file("/dev/ttyACM0");
	m_gylo_file.read((char *)data,28);
	if(m_gylo_file.bad())	return false;
*/

	for(int i=0;i<28;i++){
		cerr << (int)data[i] << " ";
	}
	cerr << endl;

	return false;


	if(data[0] != 0xff) return false;
	if(data[1] != 0xff) return false;
	if(data[2] != 'R') return false;
	if(data[3] != 'T') return false;
	if(data[4] != 0x39) return false;
	if(data[5] != 0x41) return false;
	cerr << (int)data[0] << " " << (int)data[1] << " ";
	cerr << (int)data[2] << " " << (int)data[3] << " ";
	cerr << (int)data[4] << " " << (int)data[5] << " ";
	cerr << (int)data[6] << " " << (int)data[7] << endl;
	if(data[6] != 0x05) return false;

	int x = (int)data[17]*256 + (int)data[16];
	if(x >= 32767)
		x = x - 65535;
	int y = (int)data[19]*256 + (int)data[18];
	if(y >= 32767)
		y = y - 65535;
	int z = (int)data[21]*256 + (int)data[20];
	if(z >= 32767)
		z = z - 65535;

	*ans = {x,y,z};
	return true;
}

void SensorGyro::init(void)
{
	GyroData data[3];
	for(int i=0;i<3;i++){
		if(!read(&data[i]))
			i--;
	}
	m_zero.x = (data[0].x + data[1].x + data[2].x)/3;
	m_zero.y = (data[0].y + data[1].y + data[2].y)/3;
	m_zero.z = (data[0].z + data[1].z + data[2].z)/3;
	m_initialized = true;
}

void SensorGyro::update(void)
{
	if(!m_initialized){
		cerr << "gyro not initialized" << endl;
		exit(1);
	}

	GyroData ans;
	while(!read(&ans)){}
	m_cur_deg = (double)(ans.z - m_zero.z);
	cerr << m_cur_deg << endl;
}

