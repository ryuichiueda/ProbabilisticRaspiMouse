#include "ActionsWithSensors.h"
#include "SensorRange.h"
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

ActionsWithSensors::ActionsWithSensors(){}
ActionsWithSensors::~ActionsWithSensors(){}

bool ActionsWithSensors::forwardToWall(void)
{
	bool executed = false;
	cerr << "fowardToWall" << endl;
	while(1){
		SensorRange::update();
		if(SensorRange::getLF() + SensorRange::getRF() > 1000)
			return executed;

		straighten();
		Actions::forward(50);
		executed = true;
	}
}

bool ActionsWithSensors::straighten(void)
{
	bool executed = false;
	cerr << "straighten" << endl;
	while(1){
		SensorRange::update();
		int lf = SensorRange::getLF();
		int rf = SensorRange::getRF();
		int ls = SensorRange::getLS();
		int rs = SensorRange::getRS();

		if(ls + rs <= 1500) // wall at only one side
			return executed;
		cerr << ls << "\t" << rs << endl;
		if(abs(ls - rs) <= 300) // complete !!
			return executed;

		if(ls > rs)
			Actions::turn(-3);
		else
			Actions::turn(3);

		executed = true;
	}
}

bool ActionsWithSensors::findOpenSpace(void)
{
	bool executed = false;
	cerr << "findOpenSpace" << endl;

	int angle = 5;
	int ls = SensorRange::getLS();
	int rs = SensorRange::getRS();
	if(ls > rs)
		angle = -5;

	while(1){
		SensorRange::update();
		int lf = SensorRange::getLF();
		int rf = SensorRange::getRF();
		int ls = SensorRange::getLS();
		int rs = SensorRange::getRS();


		if(lf < 200 && rf < 200 && abs(lf - rf) < 200){
			return executed;
		}

		Actions::turn(angle);

		executed = true;
	}
}

bool ActionsWithSensors::rightAngle(void)
{
	bool executed = false;
	cerr << "rightAngle" << endl;

	int prev_lf = -1;
	int prev_rf = -1; 
	while(1){
		SensorRange::update();
		int lf = SensorRange::getLF();
		int rf = SensorRange::getRF();
		cerr << lf << "\t" << rf << endl;
		const int angle = 7;
/*
		int ls = SensorRange::getLS();
		int rs = SensorRange::getRS();
*/

		if(prev_lf == -1){
			if(lf > rf){
				Actions::turn(angle);
			}else if(lf < rf){
				Actions::turn(-angle);
			}else{
				return true;
			}
		}else{
			if( (lf - prev_lf)*(rf - prev_rf) < 0 ){
				Actions::turn(45);
				rightAngle();
				return true;
			}else if(lf < prev_lf && rf < prev_rf){
				return true;
			}else if(lf > rf){
				Actions::turn(angle);
			}else{
				Actions::turn(-angle);
			}
		}

		prev_lf = lf;
		prev_rf = rf;
		executed = true;
	}
}
