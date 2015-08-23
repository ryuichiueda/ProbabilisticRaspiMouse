#ifndef ACTIONS_SENSOR_H__
#define ACTIONS_SENSOR_H__

#include "Actions.h"

class ActionsWithSensors : public Actions {
public:
	ActionsWithSensors();
	virtual ~ActionsWithSensors();

	//action primitives
	static bool forwardToWall(void);
	static bool straighten(void);
	static bool findOpenSpace(void);
	static bool rightAngle(void);
};

#endif
