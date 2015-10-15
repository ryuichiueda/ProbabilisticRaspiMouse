#ifndef AGENT_AVG_H__
#define AGENT_AVG_H__

#include "Agent.h"

class ParticleFilter;

class AgentAvg : public Agent {
public:
	AgentAvg(ParticleFilter *m_pf);
	virtual ~AgentAvg();

	//action primitives
	virtual void doAction(void);
protected:
	double valueFunction(double x,double y,double t_deg);
private:
	double thetaValue(double target_dir_deg, double t_deg);
	bool canGoForward(double x,double y,double t_deg);
};

#endif
