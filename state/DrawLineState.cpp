#include "State.h"
#include "../viewport/Viewport.h"


DrawLineState::DrawLineState(string name, component* comp)
	: State(name, comp)
{
	mName = name;
	mViewport = comp->viewport;
	mScene = comp->scene;
	mCamera = comp->camera;
	mPoints = {};
	mDrawLine = false;
}

void DrawLineState::updateScene(Scene* scene)
{
	mScene = scene;
}

void DrawLineState::mousePressEvent(QMouseEvent* event)
{
	mButton = event->button();
}

void DrawLineState::mouseMoveEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mViewport->update();
}

void DrawLineState::mouseReleaseEvent(QMouseEvent* event)
{
	if (mButton == Qt::LeftButton)
	{
		mPoints.push_back(mPos);
		mDrawLine = true;

		if (mPoints.size() == 2)
		{
			Vertex* v1 = new Vertex(mCamera->setWindow(mPoints.front().toPoint()));
			Vertex* v2 = new Vertex(mCamera->setWindow(mPos.toPoint()));
			Line* line = new Line(v1, v2);
			mScene->addShape(v1);
			mScene->addShape(v2);
			mScene->addShape(line);

			// Initialization
			mPoints = {};
			mDrawLine = false;
		}
	}

	mButton = Qt::NoButton;
	mViewport->update();
}

void DrawLineState::paintEvent(QPainter* painter)
{
	if (mDrawLine && mPoints.size() != 2)
	{
		// Draw line first
		painter->setPen(QPen(Qt::darkGray, 3));
		painter->drawLine(mPoints.at(0), mPos);

		// Draw points on line
		painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
		painter->drawPoint(mPoints.at(0));
		painter->drawPoint(mPos);
	}

	painter->setPen(QPen(Qt::black));
	painter->drawText(
		mPos.x() + 15,
		mPos.y() - 15,
		QString("x: %1, y: %2")
		.arg(mCamera->setWindow(mPos.toPoint()).x(), 0, 'f', 2)
		.arg(mCamera->setWindow(mPos.toPoint()).y(), 0, 'f', 2)
	);
}
