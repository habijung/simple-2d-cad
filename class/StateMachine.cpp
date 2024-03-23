#include <QDebug>
#include "StateMachine.h"


// State 
State::State(QString* name)
{
	this->mName = name;
}

QString* State::getStateName()
{
	return this->mName;
}


// StateMachine
StateMachine::StateMachine()
{
	qDebug() << "State Machine Test";
}

void StateMachine::addState(QString* name)
{
	State* state = new State(name);
	this->mStates.push_back(state);
}

QString* StateMachine::getCurrentState()
{
	return this->mState->getStateName();
}
