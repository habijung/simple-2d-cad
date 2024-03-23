#pragma once
#include <list>
#include "State.h"

class StateMachine
{
public:
	StateMachine();

	State* CurrentState();
	State* getState(const std::string& name);
	void AddState(State* state);
	void SetState(const std::string& name);
	void DeleteState(const std::string& name);
	void Transition(const std::string& name, SelectUtils::ViewportData* data);
	void PrintAllStates();

private:
	State* mState;
	std::list<State*> mStates;
};
