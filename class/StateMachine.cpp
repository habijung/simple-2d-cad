#include <QDebug>
#include "StateMachine.h"


// State 
State::State(string name)
{
	this->mName = name;
}

string State::getStateName()
{
	return this->mName;
}


// StateMachine
StateMachine::StateMachine()
{
	this->mState = nullptr;
	this->mStates = {};
}

void StateMachine::addState(string name)
{
	// TODO: Handle duplicate names
	State* state = new State(name);
	this->mStates.push_back(state);
}

string StateMachine::getCurrentState()
{
	return this->mState->getStateName();
}

void StateMachine::printAllStates()
{
	list<State*>::iterator iter = this->mStates.begin();

	for (iter; iter != this->mStates.end(); iter++)
	{
		qDebug() << (*iter)->getStateName();
	}
}
