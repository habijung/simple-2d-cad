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
	bool checkSameName = false;

	if (this->mStates.empty())
	{
		this->mStates.push_back(state);
	}
	else
	{
		for (std::list<State*>::iterator iter = mStates.begin(); iter != mStates.end(); iter++)
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
			mStates.push_back(state);
		}
	}
}

State* StateMachine::GetState(const std::string& name)
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

void StateMachine::SetState(const std::string& name)
{
	for (std::list<State*>::iterator iter = mStates.begin(); iter != mStates.end(); iter++)
	{
		if (!(*iter)->GetName().compare(name))
		{
			mState = *iter;
			break;
		}
	}
}

void StateMachine::DeleteState(const std::string& name)
{
	bool checkSameName = false;

	for (std::list<State*>::iterator iter = mStates.begin(); iter != mStates.end(); iter++)
	{
		if (!(*iter)->GetName().compare(name))
		{
			iter = mStates.erase(iter);
			break;
		}
	}
}

void StateMachine::Transition(const std::string& name, SelectUtils::ViewportData* data)
{
	SetState(name);
	mState->UpdateScene(data->scene);
}

void StateMachine::PrintAllStates()
{
	for (std::list<State*>::iterator iter = mStates.begin(); iter != mStates.end(); iter++)
	{
		qDebug() << (*iter)->GetName();
	}
}
