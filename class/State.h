#pragma once

#include <string>
#include <QPoint>
using namespace std;

class State
{
public:
	State(string name);
	string getStateName();

	virtual ~State() {}
	virtual void mousePressEvent(QPoint p) {}
	virtual void mouseMoveEvent() {}
	virtual void mouseReleaseEvent() {}

private:
	string mName;
};

class DrawLineState : public State
{
public:
	DrawLineState(string name);

	virtual void mousePressEvent(QPoint p);
	virtual void mouseMoveEvent() {}
	virtual void mouseReleaseEvent() {}

private:
	string mName;
};
