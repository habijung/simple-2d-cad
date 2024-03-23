#include "State.h"
#include "../viewport/Viewport.h"


State::State(string name, component* comp)
{
	mName = name;
	mViewport = comp->viewport;
	mScene = comp->scene;
	mCamera = comp->camera;
}

string State::getStateName()
{
	return this->mName;
}

bool State::getMouseLeftPressed(bool hit, int button, QMouseEvent* event)
{
	return hit && (button == Qt::LeftButton) && (event->button() == Qt::NoButton);
}
