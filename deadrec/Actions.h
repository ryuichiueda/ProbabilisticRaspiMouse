#ifndef ACTIONS_H__
#define ACTIONS_H__

class Actions {
public:
	Actions();
	~Actions();

	static void inputMotor(int l_hz, int r_hz,int msec);
	static void forward(int mm);
	static void turn(int deg);
};

#endif
