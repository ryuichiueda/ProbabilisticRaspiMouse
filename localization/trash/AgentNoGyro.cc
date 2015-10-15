#include "AgentNoGyro.h"
#include "ParticleFilterGyro.h"
#include <iostream>
#include <cmath>
#include "Actions.h"
#include "SensorRange.h"
using namespace std;

AgentNoGyro::AgentNoGyro(ParticleFilter *pf) : Agent(pf)
{
}

AgentNoGyro::~AgentNoGyro()
{
}

void AgentNoGyro::doAction(void)
{
	ofstream ofs("/tmp/particles");

	m_pf->print(&ofs);
	
	for(int i=0;i<8;i++){
		SensorRange::update();
		m_pf->sensorUpdate();

		if(i==0 || i==2 || i==3 || i==5){
			Actions::forward(180);
			m_pf->motionUpdate(180.0,0.0);
		}else if(i==1 || i==4 || i==6 || i==7){
			Actions::turn(90);
			m_pf->motionUpdate(0.0,90.0);
		}
		Particle p = m_pf->getAverage();
		m_pf->print(&ofs);
		cerr << p.x_mm << '\t' << p.y_mm << '\t' << p.t_rad/3.141592*180.0 << endl;
	}
}

double AgentNoGyro::thetaValue(double target_dir_deg, double t_deg)
{
	if(target_dir_deg < 0.0 || target_dir_deg > 360.0){
		cerr << "too large target_dir_deg at AgentNoGyro::thetaValue" << endl;
		exit(1);
	}

	double delta = fabs(t_deg - target_dir_deg);
	if(delta > 180.0)
		delta = 360.0 - delta;

	return delta/5;
}

double AgentNoGyro::valueFunction(double x,double y,double t_deg)
{
	if(t_deg < 0.0 || t_deg > 360.0){
		cerr << "too large t_deg at AgentNoGyro::valueFunction" << endl;
		exit(1);
	}

	if(x >= 540.0 && y < 230.0)//bottom right
		return 4.0 + thetaValue(90.0,t_deg);

	if(x >= 540.0 && y < 360.0)//right side
		return 3.0 + thetaValue(180.0,t_deg);

	if(x >= 360.0 && y >= 180.0 && y < 310.0)//right center
		return 2.0 + thetaValue(180.0,t_deg);

	if(x >= 360.0 && y < 540.0)//right top
		return 3.0 + thetaValue(270.0,t_deg);

	if(x >= 230.0 && x < 310.0 && y >= 180.0 && y < 360.0)//upper goal
		return 1.0 + thetaValue(270.0,t_deg);

	if(x < 180.0 && y >= 310.0 && y < 540.0)//left top
		return 3.0 + thetaValue(270.0,t_deg);

	if(x < 230.0 && y >= 180.0 && y < 360.0)//left side
		return 3.0 + thetaValue(270.0,t_deg);

	if(x >= 180.0 && x < 360.0)//goal area
		return 0.0;


	return 100.0;
}

bool AgentNoGyro::canGoForward(double x,double y,double t_deg)
{
	if(t_deg < 0.0 || t_deg > 360.0){
		cerr << "too large t_deg at AgentNoGyro::valueFunction" << endl;
		exit(1);
	}

	if(x >= 540.0 && y < 230.0)//bottom right
		return thetaValue(90.0,t_deg) < 1.0;

	if(x >= 540.0 && y < 360.0)//right side
		return thetaValue(180.0,t_deg) < 1.0;

	if(x >= 360.0 && y >= 180.0 && y < 310.0)//right center
		return thetaValue(180.0,t_deg) < 1.0;

	if(x >= 360.0 && y < 540.0)//right top
		return thetaValue(270.0,t_deg) < 1.0;

	if(x >= 230.0 && x < 310.0 && y >= 180.0 && y < 360.0)//upper goal
		return thetaValue(270.0,t_deg) < 1.0;

	if(x < 180.0 && y >= 310.0 && y < 540.0)//left top
		return thetaValue(270.0,t_deg) < 1.0;

	if(x < 230.0 && y >= 180.0 && y < 360.0)//left side
		return thetaValue(270.0,t_deg) < 1.0;

	if(x >= 180.0 && x < 360.0)//goal area
		return true; 

	return false;
}
