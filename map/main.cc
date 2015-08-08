//#include <iostream>
#include <fstream>
#include "Actions.h"
#include "ParticleFilter.h"
#include <unistd.h>
using namespace std;

int main(int argc, char const* argv[])
{
	ifstream urandom("/dev/urandom");
	ParticleFilter pf(100,&urandom,argv[1]);
	exit(0);
}
