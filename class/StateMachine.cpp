#include <QDebug>
#include "StateMachine.h"


StateMachine::StateMachine()
{
	this->mState = nullptr;
	this->mStates = {};
}

void StateMachine::addState(State* state)
{
	string name = state->getStateName();
	list<State*>::iterator iter = this->mStates.begin();
	bool checkSameName = false;

	if (this->mStates.empty())
	{
		this->mStates.push_back(state);
	}
	else
	{
		for (iter; iter != this->mStates.end(); iter++)
		{
			// Check if the same name exists.
			if (!(*iter)->getStateName().compare(name))
			{
				checkSameName = true;
				break;
			}
		}

		if (!checkSameName)
		{
			this->mStates.push_back(state);
		}
	}
}

void StateMachine::setState(State* state)
{
	this->mState = state;
}

void StateMachine::transition(State* state)
{
	// TODO: Transmission QEvent with enter, leave
	this->setState(state);
}

State* StateMachine::getCurrentState()
{
	string name = this->mState->getStateName();
	//qDebug() << "Current State:" << name;

	return this->mState;
}

void StateMachine::printAllStates()
{
	list<State*>::iterator iter = this->mStates.begin();

	for (iter; iter != this->mStates.end(); iter++)
	{
		qDebug() << (*iter)->getStateName();
	}
}
