#ifndef AGENT_NO_GYRO_H__
#define AGENT_NO_GYRO_H__

#include "Agent.h"

class ParticleFilter;

class AgentNoGyro : public Agent {
public:
	AgentNoGyro(ParticleFilter *m_pf);
	virtual ~AgentNoGyro();

	//action primitives
	virtual void doAction(void);
protected:
	double valueFunction(double x,double y,double t_deg);
private:
	double thetaValue(double target_dir_deg, double t_deg);
	bool canGoForward(double x,double y,double t_deg);
};

#endif
