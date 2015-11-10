#ifndef AGENT_AVG_H__
#define AGENT_AVG_H__

#include <string>
#include <vector>
#include "Agent.h"
using namespace std;

class ParticleFilterGyro;

class AgentAvg : public Agent {
public:
	AgentAvg(int argc, char const* argv[]);
	virtual ~AgentAvg();

	//action primitives
	virtual void doAction(void);
private:
	string m_map_name;
	vector<string> m_policy;
	int m_cell_width_mm;
	int m_cell_width_deg;
	int m_cell_num_x;
	int m_cell_num_y;
	int m_cell_num_t;

	string getAction(double x_mm,double y_mm,double theta_deg);
};

#endif
