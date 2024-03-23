#include "State.h"
#include "../class/Viewport.h"



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
