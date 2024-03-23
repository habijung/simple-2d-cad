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
	std::string name = state->getStateName();
	std::list<State*>::iterator iter = this->mStates.begin();
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

State* StateMachine::getState(std::string name)
{
	std::list<State*>::iterator iter = mStates.begin();
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

void StateMachine::setState(std::string name)
{
	std::list<State*>::iterator iter = mStates.begin();
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

void StateMachine::deleteState(std::string name)
{
	std::list<State*>::iterator iter = mStates.begin();
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

void StateMachine::transition(std::string name, SelectUtils::ViewportData* data)
{
	setState(name);
	mState->updateScene(data->scene);
}

void StateMachine::printAllStates()
{
	std::list<State*>::iterator iter = this->mStates.begin();

	for (iter; iter != this->mStates.end(); iter++)
	{
		qDebug() << (*iter)->getStateName();
	}
}
