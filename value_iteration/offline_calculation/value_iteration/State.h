#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <limits.h>
using namespace std;

class State{
private:
	struct Transition{
		unsigned int probability;
		unsigned long posterior_state, cost;
	};
	struct Action{
		vector<Transition> trans;
	};

	vector<Action> m_actions;
	unsigned long m_value;
	int m_action_index;
	const static unsigned long m_value_max = UINT_MAX;
public:
	State();
	~State();
	bool setStateTrans(int a,unsigned long s_to,unsigned int p,unsigned long c,int action_num);
	unsigned long valueIteration(vector<State> &other_state,int my_index);
	unsigned long valueIterationAction(int a, vector<State> &other_state);
	//unsigned long valueIterationAction(Action *a, vector<State> &other_state);
	void setValue(unsigned long v);
	unsigned long getValue(void){return m_value;}
	int getActionIndex(void);
};

#endif
