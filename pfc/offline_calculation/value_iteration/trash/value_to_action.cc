#include <iostream>
#include <vector>
#include "StateTrans.h"
#include <thread>
using namespace std;

void usage(void){
	cerr << "\tvalue_to_action" << endl;	
	cerr << "\tusage:\t./value_iteration <state trans file>" << endl;	
	cerr << "\tdate:\tFri Aug 15 10:38:53 JST 2014" << endl;	
	cerr << "\twritten by Ryuichi Ueda" << endl;
	exit(1);
}

void die(string reason){
	cerr << "value_iteration: [ERROR] " << reason << endl;
	exit(1);
}

/*
void worker(int start_pos,StateTrans *st) {
	//no need of exclusive control due to the property of value functions
	st->valueIteration(start_pos);
}
*/

int main(int argc, char const* argv[])
{
	//handling of options
	int filename_pos = 1;
	if(argc != 2){
		usage();
	}

	StateTrans st;
	//reading data from file
	if(! st.readStateTransFile(argv[filename_pos]))
		die("state_trans file error");

	//execution with n threads
	st.printActions();

	exit(0);
}
