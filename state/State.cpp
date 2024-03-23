#include "State.h"
#include "../viewport/Viewport.h"


State::State(string name, viewportData* data)
{
	mName = name;
	mViewport = data->viewport;
	mScene = data->scene;
	mCamera = data->camera;
}

string State::getStateName()
{
	return this->mName;
}

bool State::getMouseLeftPressed(bool hit, int button, QMouseEvent* event)
{
	return hit && (button == Qt::LeftButton) && (event->button() == Qt::NoButton);
}
