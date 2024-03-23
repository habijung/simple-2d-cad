#pragma once

#include <list>
#include <string>
#include "State.h"
using namespace std;


class StateMachine
{
public:
	StateMachine();
	State* state();
	void addState(State* state);
	State* getState(string name);
	void setState(string name);
	void deleteState(string name);
	void transition(string name, viewportData* data);
	void printAllStates();

private:
	State* mState;
	list<State*> mStates;
};
