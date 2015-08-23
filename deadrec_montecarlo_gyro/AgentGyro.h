#ifndef AGENT_GYRO_H__
#define AGENT_GYRO_H__

#include "Agent.h"

class ParticleFilter;

class AgentGyro : public Agent {
public:
	AgentGyro(int argc, char const* argv[]);
	virtual ~AgentGyro();

	//action primitives
	virtual void doAction(void);
private:
};

#endif
