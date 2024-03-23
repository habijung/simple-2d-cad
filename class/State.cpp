#include <QDebug>
#include "State.h"


State::State(string name)
{
	qDebug() << "Create State:" << name;
	this->mName = name;
}

string State::getStateName()
{
	return this->mName;
}
