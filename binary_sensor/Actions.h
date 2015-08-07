#ifndef ACTIONS_H__
#define ACTIONS_H__

class Actions {
public:
	Actions();
	~Actions();

	//action primitives
	static void forward(int mm);
	static void turn(int deg);

private:
	//utility
	static void inputMotor(int l_hz, int r_hz,int msec);
};

#endif
