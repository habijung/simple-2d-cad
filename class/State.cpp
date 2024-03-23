#include "State.h"


State::State(string name)
{
	this->mName = name;
}

string State::getStateName()
{
	return this->mName;
}
