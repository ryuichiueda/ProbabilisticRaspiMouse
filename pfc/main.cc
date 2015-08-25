#include <iostream>
#include <fstream>
#include <cmath>
#include "Actions.h"
#include "Sensor.h"
#include "ParticleFilterGyro.h"
#include "AgentPfc.h"
#include <unistd.h>
#include <string>
using namespace std;

int main(int argc, char const* argv[])
{
	AgentPfc agent(argc,argv,1);
	agent.doAction();
	exit(0);
}
