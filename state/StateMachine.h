#pragma once
#include <list>
#include "State.h"

class StateMachine
{
public:
	StateMachine();
	
	State* CurrentState();
	State* getState(std::string name);
	void AddState(State* state);
	void SetState(std::string name);
	void DeleteState(std::string name);
	void Transition(std::string name, SelectUtils::ViewportData* data);
	void PrintAllStates();

private:
	State* mState;
	std::list<State*> mStates;
};
