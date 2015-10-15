#ifndef DECISION_H__
#define DECISION_H__

class ParticleFilter;

class Agent {
public:
	Agent(ParticleFilter *m_pf);
	virtual ~Agent();

	//action primitives
	virtual void doAction(void) = 0;

protected:
	ParticleFilter *m_pf;	
};

#endif
