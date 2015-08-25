#ifndef AGENT_PFC_H__
#define AGENT_PFC_H__

#include <string>
#include <vector>
#include "Agent.h"
using namespace std;

class ParticleFilterGyro;

class AgentPfc : public Agent {
public:
	AgentPfc(int argc, char const* argv[],int power);
	virtual ~AgentPfc();

	//action primitives
	virtual void doAction(void);
private:
	string m_map_name;
	vector<unsigned long> m_value;
	int m_cell_width_mm;
	int m_cell_width_deg;
	int m_cell_num_x;
	int m_cell_num_y;
	int m_cell_num_t;

        double m_init_x_mm;
        double m_init_y_mm;
        double m_init_t_deg;

	void stateTransition(double x_mm,double y_mm,double theta_rad,
				double fw, double rad,
				double *ax_mm,double *ay_mm,double *atheta_rad);
	double getValue(double x_mm,double y_mm,double theta_deg);
	int getIndex(double x_mm,double y_mm,double theta_deg);
	string getAction(ParticleFilterGyro *pf);
	int m_power;
};

#endif
