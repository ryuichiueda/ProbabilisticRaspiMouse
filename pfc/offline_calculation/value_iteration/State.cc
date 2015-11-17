#include "State.h"
using namespace std;

State::State()
{
	m_action_index = -1;
	m_value = m_value_max;
}

State::~State(){}

bool State::setStateTrans(int a,unsigned long s_to,unsigned int p,unsigned long c,int action_num)
{
	if(m_actions.size() == 0){
		Action act;
		for(int i=0;i<action_num;i++)
			m_actions.push_back(act);
	}

	Transition t{p,s_to,c};
	m_actions[a].trans.push_back(t);
	return true;
}

unsigned long State::valueIteration(vector<State> &other_state,int my_index)
{
	unsigned long v_min = m_value;

	if(my_index == 12690)
		cerr << this << endl;

	bool is_final_state = true;
	for(unsigned int a=0;a<m_actions.size();a++){
		if(m_actions[a].trans.size() == 0)
			continue;

		is_final_state = false;
		unsigned long v = valueIterationAction(a,other_state);

		if(v_min >= v){
			v_min = v;
			m_action_index = a;

			if(my_index == 12690)
				cerr << "action change " << m_action_index << endl;
		}
	}

	if(is_final_state){
		m_action_index = -1;//final state
		if(my_index == 12690)
			cerr << "action change " << m_action_index << endl;
	}
	return v_min;
}

unsigned long State::valueIterationAction(int a,vector<State> &other_state)
{
	auto *trans = &m_actions[a].trans;

	unsigned long v = 0;
	for(auto &i : *trans){
		unsigned long tmp = other_state[i.posterior_state].getValue() + (i.cost);
		v += tmp * i.probability;
	}
	return v >> 16;
}

void State::setValue(unsigned long v)
{
	m_value = v;
}

int State::getActionIndex(void)
{
	return m_action_index;
}
