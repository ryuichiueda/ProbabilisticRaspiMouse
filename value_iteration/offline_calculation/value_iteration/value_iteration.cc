#include <iostream>
#include <vector>
#include "StateTrans.h"
#include <thread>
using namespace std;

void usage(void){
	cerr << "\tvalue_iteration" << endl;	
	cerr << "\tusage:\t./value_iteration [-p <n>] <state trans file>" << endl;	
	cerr << "\tdate:\tMon Aug 11 12:31:24 JST 2014" << endl;	
	cerr << "\twritten by Ryuichi Ueda" << endl;
	exit(1);
}

void die(string reason){
	cerr << "value_iteration: [ERROR] " << reason << endl;
	exit(1);
}

void worker(int start_pos,StateTrans *st) {
	//no need of exclusive control due to the property of value functions
	st->valueIteration(start_pos);
}

int main(int argc, char const* argv[])
{
	//handling of options
	int worker_num = 1;
	int filename_pos = 1;
	if(argc < 2 || argc == 3 || argc > 4){
		usage();
	}
	else if(argc == 4){
		filename_pos = 3;
		worker_num = atoi(argv[2]);

		if(worker_num <= 0)
			die("Invalid Thread Num");
	}

	StateTrans st;
	//reading data from file
	if(! st.readStateTransFile(argv[filename_pos]))
		die("state_trans file error");

	//execution with n threads
	vector<thread> th;
	for(int n=0;n<worker_num;n++){
		unsigned long start_pos = (unsigned long)
			(double(st.getStateNum())/worker_num*n);


		th.push_back(thread(worker,start_pos,&st));
	}

	//waiting
	for(int n=0;n<worker_num;n++){
		th[n].join();
	}

	st.printValues();

	if(st.isConverged()){
		st.printActions();
	}
	exit(0);
}
