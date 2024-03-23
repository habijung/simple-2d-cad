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

DrawFaceState::DrawFaceState(string name, component* comp)
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

void DrawFaceState::updateScene(Scene* scene)
{
	mScene = scene;
}

void DrawFaceState::mousePressEvent(QMouseEvent* event)
{
	mButton = event->button();
}

void DrawFaceState::mouseMoveEvent(QMouseEvent* event)
{
	mPos = event->pos();

	if (mPoints.size() && ((mButton != Qt::LeftButton) || (mButton == Qt::LeftButton)))
	{
		// Create bounding box and hit testing by start point.
		mLine = QLineF(mPoints.back(), mPos);
		mVertex = new Vertex(mCamera->setWindow(mPoints[0].toPoint()));
		mPolygon = createPointBoundingBox(mCamera, mVertex, 15); // Start point의 bounding box
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

void DrawFaceState::mouseReleaseEvent(QMouseEvent* event)
{
	if (mButton == Qt::LeftButton)
	{
		mPoints.push_back(mPos);
		mDrawPolygon = true;

		// Draw finish and Initialization
		if (mHit && (mPoints.size() > 2))
		{
			// TODO: Polygon을 Scene에 저장하기
			if (mPoints.size() != 3)
			{
				Vertex* v;
				list<Vertex*> vertices;

				// Add vertices to Scene
				// 마지막 점은 polygon 구성에서 빼기
				for (int i = 0; i < mPoints.size() - 1; i++)
				{
					v = new Vertex(mCamera->setWindow(mPoints[i].toPoint()));
					vertices.push_back(v);
					mScene->addShape(v);
				}

				mScene->addShape(new Face(vertices));
			}

			mPoints = {};
			mDrawPolygon = false;
			mGuidePolygon = QPolygonF();
		}
	}

	mButton = Qt::NoButton;
	mViewport->update();
}

void DrawFaceState::paintEvent(QPainter* painter)
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

	painter->setPen(QPen(Qt::black));
	painter->drawText(
		mPos.x() + 15,
		mPos.y() - 15,
		QString("x: %1, y: %2")
		.arg(mCamera->setWindow(mPos.toPoint()).x(), 0, 'f', 2)
		.arg(mCamera->setWindow(mPos.toPoint()).y(), 0, 'f', 2)
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
	mSnap = false;
}

void SelectPointState::updateScene(Scene* scene)
{
	mScene = scene;
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
			if ((*iter)->checkType("Point"))
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

	if (getMouseLeftPressed(mHit, mButton, event) && mShapes.size())
	{
		mVertex->updateVertex(mCamera->setWindow(mPos.toPoint()));

		// Check snapping
		list<Shape*>::iterator iter = mShapes.begin();

		for (iter; iter != mShapes.end(); iter++)
		{
			if ((*iter)->checkType("Point") && ((*iter) != mVertex))
			{
				mSnapVertex = dynamic_cast<Vertex*>(*iter)->retVertex();
				mSnapPolygon = createPointBoundingBox(mCamera, &mSnapVertex, 10);
				mSnap = hitTestingPoint(mPos, mSnapPolygon);

				if (mSnap)
				{
					// Snap이 발생하면 현재 Vertex를 해당 위치로 업데이트
					QPointF p = mSnapVertex.retVertex();
					mVertex->updateVertex(p);
					mSnapPoint = mCamera->setScreen(p);
					break;
				}
			}
		}

		// Snap 발생과 상관없이 Vertex의 마지막 위치에서 bounding box 생성
		mPolygon = createPointBoundingBox(mCamera, mVertex, 10);
	}

	mViewport->update();
}

void SelectPointState::mouseReleaseEvent(QMouseEvent* event)
{
	mButton = Qt::NoButton;
	mSnap = false;
}

void SelectPointState::paintEvent(QPainter* painter)
{
	if (mHit)
	{
		// Vertex highlight
		painter->setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
		painter->drawPoint(mCamera->setScreen(mVertex->retVertex()));

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

void SelectLineState::updateScene(Scene* scene)
{
	mScene = scene;
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
			if ((*iter)->checkType("Line"))
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
		mLine->updateLine(mCamera, mPosStart, mPos, mLineVertices);
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

SelectFaceState::SelectFaceState(string name, component* comp)
	: State(name, comp)
{
	mName = name;
	mViewport = comp->viewport;
	mScene = comp->scene;
	mCamera = comp->camera;
	mFace = new Face(new Vertex(QPointF(INFINITY, INFINITY)));
}

void SelectFaceState::updateScene(Scene* scene)
{
	mScene = scene;
}

void SelectFaceState::mousePressEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mPosStart = event->pos();
	mFaceVertices = mFace->retVertices();
	mButton = event->button();
}

void SelectFaceState::mouseMoveEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mShapes = mScene->retShapes();

	if (mButton != Qt::LeftButton)
	{
		list<Shape*>::reverse_iterator iter = mShapes.rbegin();

		for (iter; iter != mShapes.rend(); iter++)
		{
			if ((*iter)->checkType("Face"))
			{
				// No need to consider square bounding box type
				mFace = static_cast<Face*>(*iter);
				mPolygon = mFace->retFace(mCamera);
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
		mFace->updateFace(mCamera, mPosStart, mPos, mScene->retShapes(), mFaceVertices);
		mPolygon = mFace->retFace(mCamera);
	}

	mViewport->update();
}

void SelectFaceState::mouseReleaseEvent(QMouseEvent* event)
{
	mButton = Qt::NoButton;
}

void SelectFaceState::paintEvent(QPainter* painter)
{
	if (mHit)
	{
		// Face line highlight
		painter->setPen(QPen(Qt::red, 3));
		painter->setBrush(QBrush(Qt::cyan));
		painter->drawPolygon(mPolygon);

		// Draw polygon points again for priority
		painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));

		for (int i = 0; i < mPolygon.size(); i++)
		{
			painter->drawPoint(QPoint(mPolygon[i].toPoint()));
		}

		// Draw selection box
		mPolygon = QPolygonF();
		mPolygon = createFaceSelectionBox(mCamera, mFace);
		painter->setPen(QPen(Qt::green, 3));
		painter->setBrush(QBrush());
		painter->drawPolygon(mPolygon);
	}
}
