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
	mP1 = event->pos();
	mButton = event->button();

	if (mButton == Qt::LeftButton)
	{
		float dx = mViewport->width() / 2.0;
		float dy = mViewport->height() / 2.0;
		float scale = 100.0;
		mV1 = new Vertex(
			mCamera->setScreenToWindow(mP1, dx, dy, scale).x(),
			mCamera->setScreenToWindow(mP1, dx, dy, scale).y()
		);
		mScene->addShape(mV1);
	}

	mViewport->update();
}

void DrawLineState::mouseMoveEvent(QMouseEvent* event)
{
	mOffsetX = event->pos().x() - mP1.x();
	mOffsetY = event->pos().y() - mP2.y();

	mViewport->update();
}

void DrawLineState::mouseReleaseEvent(QMouseEvent* event)
{
	mP2 = event->pos();

	if (mButton == Qt::LeftButton)
	{
		float dx = mViewport->width() / 2.0;
		float dy = mViewport->height() / 2.0;
		float scale = 100.0;
		mV2 = new Vertex(
			mCamera->setScreenToWindow(mP2, dx, dy, scale).x(),
			mCamera->setScreenToWindow(mP2, dx, dy, scale).y()
		);
		mScene->addShape(mV2);

		Line* l = new Line(mV1, mV2);
		mScene->addShape(l);

		mOffsetX = 0;
		mOffsetY = 0;
		mButton = Qt::NoButton;
	}

	mViewport->update();
}

void DrawLineState::paintEvent(QPainter* painter)
{
	QPen pen(Qt::green, 10);
	painter->setPen(pen);

	Point start = { mP1.x(), mP1.y() };
	Point end = { mP2.x(), mP2.y() };

	if (start.x | start.y)
	{
		QPen penLine(Qt::red, 2);
		painter->setPen(penLine);
		painter->drawLine(start.x, start.y, end.x, end.y);

		QPen penText(Qt::darkGray, 3);
		painter->setPen(penText);
		painter->drawText(
			start.x + 10,
			start.y,
			QString("x: %1, y: %2").arg(start.x).arg(start.y)
		);
		painter->drawText(
			end.x + 10,
			end.y,
			QString("x: %1, y: %2").arg(end.x).arg(end.y)
		);
	}
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

	// TODO: Modify addVertex parameter
	//this->mScene->addVertex(this->mCamera->setScreenToWindow(p, dx, dy, scale));
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
