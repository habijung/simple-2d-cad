#include "State.h"
#include "../viewport/Viewport.h"


SelectFaceState::SelectFaceState(string name, viewportData* data)
	: State(name, data)
{
	mName = name;
	mViewport = data->viewport;
	mScene = data->scene;
	mCamera = data->camera;
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
