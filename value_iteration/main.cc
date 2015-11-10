#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
#include "Sensor.h"
#include "ParticleFilterGyro.h"
#include "AgentAvg.h"
#include <unistd.h>
#include <string>
using namespace std;

int main(int argc, char const* argv[])
{
	AgentAvg agent(argc,argv);
	agent.doAction();
	exit(0);
}
