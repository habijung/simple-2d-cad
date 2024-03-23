#include <QDebug>
#include "StateMachine.h"


StateMachine::StateMachine()
{
	this->mState = nullptr;
	this->mStates = {};
}

State* StateMachine::state()
{
	return mState;
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

State* StateMachine::getState(string name)
{
	list<State*>::iterator iter = mStates.begin();
	bool checkSameName = false;

	for (iter; iter != mStates.end(); iter++)
	{
		if (!(*iter)->getStateName().compare(name))
		{
			checkSameName = true;
			break;
		}
	}

	if (checkSameName)
	{
		return *iter;
	}
	else
	{
		return mStates.front(); // State: SELECT_POINT
	}
}

void StateMachine::setState(string name)
{
	list<State*>::iterator iter = mStates.begin();
	bool checkSameName = false;

	for (iter; iter != mStates.end(); iter++)
	{
		if (!(*iter)->getStateName().compare(name))
		{
			checkSameName = true;
			break;
		}
	}

	if (checkSameName)
	{
		mState = *iter;
	}
}

void StateMachine::deleteState(string name)
{
	list<State*>::iterator iter = mStates.begin();
	bool checkSameName = false;

	for (iter; iter != mStates.end(); iter++)
	{
		if (!(*iter)->getStateName().compare(name))
		{
			iter = mStates.erase(iter);
			break;
		}
	}
}

void StateMachine::transition(string name, component* comp)
{
	setState(name);
	mState->updateScene(comp->scene);
}

void StateMachine::printAllStates()
{
	list<State*>::iterator iter = this->mStates.begin();

	for (iter; iter != this->mStates.end(); iter++)
	{
		qDebug() << (*iter)->getStateName();
	}
}
