#ifndef DECISION_H__
#define DECISION_H__

class Agent {
public:
	Agent(int argc, char const* argv[]){}
	virtual ~Agent(){}

	//action primitives
	virtual void doAction(void) = 0;

protected:
};

#endif
