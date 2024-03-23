#include <QDebug>
#include "StateMachine.h"

StateMachine::StateMachine()
{
	this->mState = nullptr;
	this->mStates = {};
}

State* StateMachine::CurrentState()
{
	return mState;
}

void StateMachine::AddState(State* state)
{
	std::string name = state->GetName();
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
			if (!(*iter)->GetName().compare(name))
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
		if (!(*iter)->GetName().compare(name))
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
		// Return default state: SELECT_POINT
		return mStates.front();
	}
}

void StateMachine::SetState(std::string name)
{
	std::list<State*>::iterator iter = mStates.begin();
	bool checkSameName = false;

	for (iter; iter != mStates.end(); iter++)
	{
		if (!(*iter)->GetName().compare(name))
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

void StateMachine::DeleteState(std::string name)
{
	std::list<State*>::iterator iter = mStates.begin();
	bool checkSameName = false;

	for (iter; iter != mStates.end(); iter++)
	{
		if (!(*iter)->GetName().compare(name))
		{
			iter = mStates.erase(iter);
			break;
		}
	}
}

void StateMachine::Transition(std::string name, SelectUtils::ViewportData* data)
{
	SetState(name);
	mState->UpdateScene(data->scene);
}

void StateMachine::PrintAllStates()
{
	std::list<State*>::iterator iter = this->mStates.begin();

	for (iter; iter != this->mStates.end(); iter++)
	{
		qDebug() << (*iter)->GetName();
	}
}
