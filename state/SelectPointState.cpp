#include "State.h"
#include "../class/Viewport.h"


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
