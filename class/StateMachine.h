#pragma once

#include <list>
#include <string>
#include "State.h"
using namespace std;


class StateMachine
{
public:
	StateMachine();
	void addState(State* state);
	void setState(State* state);
	void transition(State* state);
	State* getCurrentState();

	void printAllStates();

private:
	State* mState;
	list<State*> mStates;
};
