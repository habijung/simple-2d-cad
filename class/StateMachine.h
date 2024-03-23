#pragma once

#include <list>
#include <string>
#include "State.h"
using namespace std;


class StateMachine
{
public:
	StateMachine();
	void addState(string name);
	void setState(string name);
	void transitionState(State* state);
	string getCurrentState();

	void printAllStates();

private:
	State* mState;
	list<State*> mStates;
};
