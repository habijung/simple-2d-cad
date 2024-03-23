#include <QDebug>
#include "StateMachine.h"


StateMachine::StateMachine()
{
	this->mState = nullptr;
	this->mStates = {};
}

void StateMachine::addState(string name)
{
	State* state = new State(name);
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

void StateMachine::setState(string name)
{
	list<State*>::iterator iter = this->mStates.begin();

	for (iter; iter != this->mStates.end(); iter++)
	{
		if (!(*iter)->getStateName().compare(name))
		{
			this->mState = *iter;
			break;
		}
	}
}

string StateMachine::getCurrentState()
{
	string name = this->mState->getStateName();
	qDebug() << "Current State:" << name;
	return name;
}

void StateMachine::printAllStates()
{
	list<State*>::iterator iter = this->mStates.begin();

	for (iter; iter != this->mStates.end(); iter++)
	{
		qDebug() << (*iter)->getStateName();
	}
}
