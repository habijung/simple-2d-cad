#include "State.h"
#include "../viewport/Viewport.h"

State::State(const std::string& name, SelectUtils::ViewportData* data)
{
	mName = name;
	mViewport = data->viewport;
	mScene = data->scene;
	mCamera = data->camera;
}

std::string State::GetName()
{
	return this->mName;
}

bool State::GetMouseLeftPressed(bool hit, int button, QMouseEvent* event)
{
	return hit && (button == Qt::LeftButton) && (event->button() == Qt::NoButton);
}
