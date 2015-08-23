#include "AgentGyro.h"
#include "DeadRecMonteCarlo.h"
using namespace std;

int main(int argc, char const* argv[])
{
	AgentGyro agent(argc,argv);
	agent.doAction();
	
	exit(0);
}
