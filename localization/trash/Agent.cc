#include "Agent.h"
#include "ParticleFilter.h"
using namespace std;

Agent::Agent(ParticleFilter *pf)
{
	m_pf = pf;
}

Agent::~Agent()
{
}
