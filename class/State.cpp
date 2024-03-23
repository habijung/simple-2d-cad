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
	mPoints = {};
	mDrawPolygon = false;
	mVertex = new Vertex(QPoint(INFINITY, INFINITY));
}

void DrawPolygonState::mousePressEvent(QMouseEvent* event)
{
	mButton = event->button();
}

void DrawPolygonState::mouseMoveEvent(QMouseEvent* event)
{
	mPos = event->pos();

	if (mPoints.size() && ((mButton != Qt::LeftButton) || (mButton == Qt::LeftButton)))
	{
		// Create bounding box and hit testing by start point.
		float dx = mViewport->width() / 2.0;
		float dy = mViewport->height() / 2.0;
		float scale = 100.0;

		mLine = QLineF(mPoints.back(), mPos);
		mVertex = new Vertex(mCamera->setScreenToWindow(mPoints[0].toPoint(), dx, dy, scale));
		mPolygon = createPointBoundingBox(mCamera, mVertex, 15); // 편의를 위해서 box를 크게 만들기
		mHit = hitTestingPoint(mPos, mPolygon);

		if (mHit && (mPoints.size() > 1))
		{
			mGuidePolygon = QPolygonF();

			for (int i = 0; i < mPoints.size(); i++)
			{
				mGuidePolygon << mPoints[i];
			}

			mGuidePolygon << mPos;
		}
	}

	mViewport->update();
}

void DrawPolygonState::mouseReleaseEvent(QMouseEvent* event)
{
	if (mButton == Qt::LeftButton)
	{
		mPoints.push_back(mPos);
		mDrawPolygon = true;

		// Draw finish and Initialization
		if (mHit && (mPoints.size() > 2))
		{
			// TODO: Polygon을 Scene에 저장하기
			if (mPoints.size() != 3) {}

			mPoints = {};
			mDrawPolygon = false;
			mGuidePolygon = QPolygonF();
		}
	}

	mButton = Qt::NoButton;
	mViewport->update();
}

void DrawPolygonState::paintEvent(QPainter* painter)
{
	if (mDrawPolygon)
	{
		painter->setPen(QPen(Qt::darkGray, 3));
		painter->drawLine(mLine);

		for (int i = 0; i < mPoints.size(); i++)
		{
			// Draw polygon lines
			int j = (i + 1) % static_cast<int>(mPoints.size());

			if (j > 0)
			{
				mLine = QLineF(mPoints[i], mPoints[j]);
				painter->setPen(QPen(Qt::darkGray, 3));
				painter->drawLine(mLine);
			}

			// Draw polygon points
			painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
			painter->drawPoint(mPoints[i]);
			painter->drawPoint(mPos);
		}

		if (mHit && (mPoints.size() > 1))
		{
			// Draw guide polygon
			painter->setPen(QPen(Qt::darkGray, 3));
			painter->setBrush(QBrush(Qt::lightGray));
			painter->drawPolygon(mGuidePolygon, Qt::OddEvenFill);

			// Draw polygon points again for priority
			for (int i = 0; i < mPoints.size(); i++)
			{
				// Draw polygon points
				painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
				painter->drawPoint(mPoints[i]);
				painter->drawPoint(mPos);
			}

			// Draw mouse hover points
			painter->setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
			painter->setBrush(QBrush());
			painter->drawPoint(mPoints[0]);
			painter->drawPoint(mPos);

			// Draw bounding box with start point
			painter->setPen(QPen(Qt::blue, 2));
			painter->drawPolygon(mPolygon);
		}
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

	if (mButton != Qt::LeftButton)
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
