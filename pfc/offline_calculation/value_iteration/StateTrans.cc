#include "StateTrans.h"
#include <climits>
#include <fstream>
#include <sstream>
#include "State.h"
using namespace std;

StateTrans::StateTrans()
{
	m_converged = true;
}

StateTrans::~StateTrans(){}

bool StateTrans::setStateNum(const string &str)
{
	m_state_num = atoi(str.c_str());
	if(m_state_num <= 0 or m_states.size() > 0)
		return false;

	m_states.reserve(m_state_num);
	for(unsigned long i=0;i<m_state_num;i++){
		State s;

		m_states.push_back(s);
	}
	return true;
}

unsigned long StateTrans::getStateNum(void)
{
	return m_state_num;
}

bool StateTrans::setAction(const string &action)
{
	m_actions.push_back(action);
	return true;
}

void StateTrans::status(void)
{
	cerr << "statenum: " << m_state_num << endl;
	cerr << "actions: ";
	for(auto i=m_actions.begin();i<m_actions.end();i++){
		cerr << *i << " ";
	}
	cerr << endl;
}

State* StateTrans::getState(unsigned long index)
{
	return &m_states[index];
}

unsigned int StateTrans::getActionIndex(string &line)
{
	for(unsigned int i=0;i<m_actions.size();i++){
		if(m_actions[i] == line)
			return i;
	}
	return -1;
}

bool StateTrans::setStateTrans(unsigned long s,int a,unsigned long s_to,double prob,unsigned long cost)
{
	return m_states[s].setStateTrans(a,s_to, (unsigned int)(prob*65536),
				cost,m_actions.size());
}

bool StateTrans::valueIteration(unsigned long start_num)
{
	for(unsigned long i=start_num;i<m_state_num+start_num;i++){
		unsigned long index = (i + m_state_num)%m_state_num;
		unsigned long v = m_states[index].valueIteration(m_states,index);
		unsigned long prv = m_states[index].getValue();

		if(prv != v)
			m_converged = false;

		m_states[index].setValue(v);
	}
	return true;
}

bool StateTrans::setValue(unsigned long s,unsigned long v)
{
	m_states[s].setValue(v);
	return true;
}

void StateTrans::printValues(void)
{
	for(unsigned long i=0;i<m_state_num;i++){
		cout << i << " " << m_states[i].getValue() << endl;
	}
}

void StateTrans::printActions(void)
{
	ofstream ofs("policy");
	for(unsigned long i=0;i<m_state_num;i++){
		int a = m_states[i].getActionIndex();
		if(a >= 0)
			ofs << i << " " << m_actions[a] << endl;

		if(i==12690){
			cerr << m_states[i].getValue() << " " << a << " " << m_actions[a] << endl;
			cerr << &m_states[i] << endl;
		}

	}
}

bool StateTrans::readStateTransFile(const char *filename)
{
	ifstream ifs_state_trans(filename);
	string buf;

	//parse of header in state transition file
	while(ifs_state_trans && getline(ifs_state_trans,buf)){
		if(parseHeader(buf) == false)
			break;
	}

	//checking global setting
	//g_state_trans.status();

	//parse of state transtions in state transition file
	while(ifs_state_trans && getline(ifs_state_trans,buf)){
		if(parseStateTrans(buf) == false)
			break;
	}

	ifs_state_trans.close();

	//read of state values
	while(! cin.eof()){
		unsigned long s,v;
		cin >> s >> v;
		setValue(s,v);
	}

	return true;
}

bool StateTrans::parseHeader(string &line){
	if(line == "%%state transitions%%")
		return false;
	
	vector<string> words;
	tokenizer(line,words);
	if(words.size() < 1)
		return true;

	if(words[0] == "statenum"){
		if(! setStateNum(words[1])){
			cerr << "Invalid State Number" << endl;
			return false;
		}
	}
	else if(words[0] == "actions"){
		for(auto i=++words.begin();i<words.end();i++){
			setAction(*i);
		}	
	}

	return true;
}

bool StateTrans::parseStateTrans(string &line)
{
	vector<string> words;
	tokenizer(line,words);
	if(words.size() < 1)
		return true;

	static int state_index = -1;
	static int action_index = -1;
	if(words[0] == "state"){
		state_index = atoi(words[1].c_str());
		action_index = getActionIndex(words[3]);

		if(state_index < 0){
			cerr << "Invalid State No." << endl;
			return false;
		}
	}
	else if(words[0][0] == '\t'){
		int s_after = atoi(words[1].c_str());
		double p = atof(words[3].c_str());
		double c = atof(words[5].c_str());

		if(s_after < 0){
			cerr << "Invalid Posterior State" << endl;
			return false;
		}
		if(p <= 0.0 || p > 1.0){
			cerr << "Invalid Probability" << endl;
			return false;
		}

		setStateTrans(state_index,action_index,s_after,p,c);
	}

	return true;
}

bool StateTrans::tokenizer(string &line,vector<string> &words){
	string token;
	stringstream ss(line);
	while(getline(ss,token,' ')){
		words.push_back(token);
	}
	return true;
}
