#pragma once
#include <list>
#include "State.h"


class StateMachine
{
public:
	StateMachine();
	State* state();
	void addState(State* state);
	State* getState(std::string name);
	void setState(std::string name);
	void deleteState(std::string name);
	void transition(std::string name, SelectUtils::ViewportData* data);
	void printAllStates();

private:
	State* mState;
	std::list<State*> mStates;
};
