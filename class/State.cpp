#include <QDebug>
#include <QWidget>
#include "State.h"
#include "Viewport.h"


State::State(string name)
{
	qDebug() << "Create State:" << name;
	this->mName = name;
}

string State::getStateName()
{
	return this->mName;
}

DrawLineState::DrawLineState(string name, metadata* data)
	: State(name)
{
	qDebug() << "Create DrawLineState:" << name;
	this->mName = name;
	this->mViewport = data->viewport;
	this->mScene = data->scene;
	this->mCamera = data->camera;
}

void DrawLineState::mousePressEvent(QPoint p)
{
	qDebug() << "DrawLineState::mousePressEvent()";
	float dx = this->mViewport->width() / 2.0;
	float dy = this->mViewport->height() / 2.0;
	float scale = 100.0;
	this->mScene->addVertex(this->mCamera->setScreenToWindow(p, dx, dy, scale));
}
