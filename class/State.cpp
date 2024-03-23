#include <QDebug>
#include <QWidget>
#include "State.h"
#include "Viewport.h"


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

DrawLineState::DrawLineState(string name, component* comp)
	: State(name, comp)
{
	mName = name;
	mViewport = comp->viewport;
	mScene = comp->scene;
	mCamera = comp->camera;
}

void DrawLineState::mousePressEvent(QMouseEvent* event)
{
	mStartPoint = event->pos();
	mButton = event->button();

	if (mButton == Qt::LeftButton)
	{
		float dx = mViewport->width() / 2.0;
		float dy = mViewport->height() / 2.0;
		float scale = 100.0;
		mScene->addVertex(mCamera->setScreenToWindow(mStartPoint, dx, dy, scale));
	}
}

void DrawLineState::mouseMoveEvent(QMouseEvent* event)
{
	int xOffset = event->pos().x() - mStartPoint.x();
	int yOffset = event->pos().y() - mStartPoint.y();

	if (event->button() == Qt::NoButton)
	{
		mViewport->update();
	}
}

void DrawLineState::mouseReleaseEvent(QMouseEvent* event)
{
	mEndPoint = event->pos();

	if (mButton == Qt::LeftButton)
	{
		float dx = mViewport->width() / 2.0;
		float dy = mViewport->height() / 2.0;
		float scale = 100.0;
		mScene->addVertex(mCamera->setScreenToWindow(mEndPoint, dx, dy, scale));
	}
}

void DrawLineState::paintEvent(QPainter* painter)
{
	QPen pen(Qt::green, 10);
	painter->setPen(pen);
}

DrawFaceState::DrawFaceState(string name, component* comp)
	: State(name, comp)
{
	mName = name;
	mViewport = comp->viewport;
	mScene = comp->scene;
	mCamera = comp->camera;
}

void DrawFaceState::mousePressEvent(QPoint p)
{
	qDebug() << "DrawFaceState::mousePressEvent()";
	float dx = this->mViewport->width() / 2.0;
	float dy = this->mViewport->height() / 2.0;
	float scale = 100.0;
	this->mScene->addVertex(this->mCamera->setScreenToWindow(p, dx, dy, scale));
}

void DrawFaceState::paintEvent(QPainter* painter)
{
	QPen pen(Qt::red, 10);
	painter->setPen(pen);
}

SelectPointState::SelectPointState(string name, component* comp)
	: State(name, comp)
{
	mName = name;
	mViewport = comp->viewport;
	mScene = comp->scene;
	mCamera = comp->camera;
}
