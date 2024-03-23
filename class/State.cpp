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

DrawLineState::DrawLineState(string name)
	: State(name)
{
	qDebug() << "Create DrawLineState:" << name;
	this->mName = name;
}

void DrawLineState::mousePressEvent()
{
	qDebug() << "DrawLineState::mousePressEvent()";
}
