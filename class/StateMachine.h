#pragma once
#include <list>
using namespace std;


class State
{
public:
	State(QString* name);
	QString* getStateName();

private:
	QString* mName;
};


class StateMachine
{
public:
	StateMachine();
	void addState(QString* name);
	void setInitialState(State* state);
	void transitionState(State* state);
	QString* getCurrentState();

private:
	State* mState;
	list<State*> mStates;
};
