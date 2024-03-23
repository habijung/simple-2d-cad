#pragma once
#include <list>
#include <string>
using namespace std;


class State
{
public:
	State(string name);
	string getStateName();

private:
	string mName;
};


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
