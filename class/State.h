#pragma once

#include <string>
using namespace std;

class State
{
public:
	State(string name);
	string getStateName();

	virtual ~State() {}
	virtual void mousePressEvent() {}
	virtual void mouseMoveEvent() {}
	virtual void mouseReleaseEvent() {}

private:
	string mName;
};

class DrawLineState : public State
{
public:
	DrawLineState(string name);

	virtual void mousePressEvent();
	virtual void mouseMoveEvent() {}
	virtual void mouseReleaseEvent() {}

private:
	string mName;
};
