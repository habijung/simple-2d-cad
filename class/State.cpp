#include <cmath>
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

bool State::getMouseLeftPressed(bool hit, int button, QMouseEvent* event)
{
	return hit && (button == Qt::LeftButton) && (event->button() == Qt::NoButton);
}

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
			float dx = mViewport->width() / 2.0;
			float dy = mViewport->height() / 2.0;
			float scale = 100.0;

			Vertex* v1 = new Vertex(mCamera->setScreenToWindow(mPoints.front().toPoint(), dx, dy, scale));
			Vertex* v2 = new Vertex(mCamera->setScreenToWindow(mPos.toPoint(), dx, dy, scale));
			Line* line = new Line(v1, v2);
			mScene->addShape(v1);
			mScene->addShape(v2);
			mScene->addShape(line);

			// Initialization
			mPoints = {};
			mDrawLine = false;
			mButton = Qt::NoButton;
		}
	}

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

	float dx = mViewport->width() / 2.0;
	float dy = mViewport->height() / 2.0;
	float scale = 100.0;

	painter->setPen(QPen(Qt::black));
	painter->drawText(
		mPos.x() + 15,
		mPos.y() - 15,
		QString("x: %1, y: %2")
		.arg(mCamera->setScreenToWindow(mPos.toPoint(), dx, dy, scale).x(), 0, 'f', 2)
		.arg(mCamera->setScreenToWindow(mPos.toPoint(), dx, dy, scale).y(), 0, 'f', 2)
	);
}

DrawPolygonState::DrawPolygonState(string name, component* comp)
	: State(name, comp)
{
	mName = name;
	mViewport = comp->viewport;
	mScene = comp->scene;
	mCamera = comp->camera;
	mDrawPolygon = false;
}

void DrawPolygonState::mousePressEvent(QMouseEvent* event)
{
	mButton = event->button();
}

void DrawPolygonState::mouseMoveEvent(QMouseEvent* event)
{
	mPos = event->pos();
}

void DrawPolygonState::mouseReleaseEvent(QMouseEvent* event)
{}

void DrawPolygonState::paintEvent(QPainter* painter)
{}

SelectPointState::SelectPointState(string name, component* comp)
	: State(name, comp)
{
	mName = name;
	mViewport = comp->viewport;
	mScene = comp->scene;
	mCamera = comp->camera;
	mVertex = new Vertex(QPointF(INFINITY, INFINITY));
}

void SelectPointState::mousePressEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mButton = event->button();
}

void SelectPointState::mouseMoveEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mShapes = mScene->retShapes();

	if (!(mButton == Qt::LeftButton))
	{
		list<Shape*>::reverse_iterator iter = mShapes.rbegin();

		for (iter; iter != mShapes.rend(); iter++)
		{
			if (!(*iter)->retType().compare("Point"))
			{
				mVertex = static_cast<Vertex*>(*iter);
				mPolygon = createPointBoundingBox(mCamera, mVertex, 10);
				mHit = hitTestingPoint(mPos, mPolygon);

				if (mHit)
				{
					break;
				}
			}
		}
	}

	if (getMouseLeftPressed(mHit, mButton, event))
	{
		float dx = this->mViewport->width() / 2.0;
		float dy = this->mViewport->height() / 2.0;
		float scale = 100.0;

		mVertex->updateVertex(mCamera->setScreenToWindow(mPos.toPoint(), dx, dy, scale));
		mPolygon = createPointBoundingBox(mCamera, mVertex, 10);
	}

	mViewport->update();
}

void SelectPointState::mouseReleaseEvent(QMouseEvent* event)
{
	mButton = Qt::NoButton;
}

void SelectPointState::paintEvent(QPainter* painter)
{
	if (mHit)
	{
		// Vertex highlight
		painter->setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
		painter->drawPoint(mCamera->setWindowToScreen(mVertex->retVertex()));

		// Draw bounding box
		painter->setPen(QPen(Qt::blue, 2));
		painter->drawPolygon(mPolygon);
	}
}

SelectLineState::SelectLineState(string name, component* comp)
	: State(name, comp)
{
	mName = name;
	mViewport = comp->viewport;
	mScene = comp->scene;
	mCamera = comp->camera;
	mLine = new Line(QPointF(INFINITY, INFINITY), QPointF(INFINITY, INFINITY));
}

void SelectLineState::mousePressEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mPosStart = event->pos();
	mLineVertices = mLine->retVertices();
	mButton = event->button();
}

void SelectLineState::mouseMoveEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mShapes = mScene->retShapes();

	if (!(mButton == Qt::LeftButton))
	{
		list<Shape*>::reverse_iterator iter = mShapes.rbegin();

		for (iter; iter != mShapes.rend(); iter++)
		{
			if (!(*iter)->retType().compare("Line"))
			{
				mLine = static_cast<Line*>(*iter);
				mPolygon = createLineBoundingBox(mCamera, mLine, 10);
				mHit = hitTestingPoint(mPos, mPolygon);

				if (mHit)
				{
					break;
				}
			}
		}
	}

	if (getMouseLeftPressed(mHit, mButton, event))
	{
		float dx = this->mViewport->width() / 2.0;
		float dy = this->mViewport->height() / 2.0;
		float scale = 100.0;

		mLine->updateLine(mCamera, mPosStart, mPos, mLineVertices, dx, dy, scale);
		mPolygon = createLineBoundingBox(mCamera, mLine, 10);
	}

	mViewport->update();
}

void SelectLineState::mouseReleaseEvent(QMouseEvent* event)
{
	mButton = Qt::NoButton;
}

void SelectLineState::paintEvent(QPainter* painter)
{
	if (mHit)
	{
		// Line highlight
		painter->setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap));
		painter->drawLine(mLine->retLine(mCamera));

		// Draw bounding box
		mPolygon = createLineBoundingBox(mCamera, mLine, 10);
		painter->setPen(QPen(Qt::blue, 3));
		painter->drawPolygon(mPolygon);

		// Draw selection box
		mPolygon = createLineSelectionBox(mCamera, mLine);
		painter->setPen(QPen(Qt::green, 3));
		painter->drawPolygon(mPolygon);
	}
}
