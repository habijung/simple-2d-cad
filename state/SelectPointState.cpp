#include "State.h"
#include "../viewport/Viewport.h"

SelectPointState::SelectPointState(std::string name, SelectUtils::ViewportData* data)
	: State(name, data)
{
	mName = name;
	mViewport = data->viewport;
	mScene = data->scene;
	mCamera = data->camera;
	mVertex = new Vertex(QPointF(INFINITY, INFINITY));
	mSnap = false;
}

void SelectPointState::UpdateScene(Scene* scene)
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
		std::list<Shape*>::reverse_iterator iter = mShapes.rbegin();

		for (iter; iter != mShapes.rend(); iter++)
		{
			if ((*iter)->CheckShapeType("Point"))
			{
				mVertex = static_cast<Vertex*>(*iter);
				mPolygon = SelectUtils::CreatePointBoundingBox(mCamera, mVertex, 10);
				mHit = SelectUtils::HitTesting(mPos, mPolygon);

				if (mHit)
				{
					break;
				}
			}
		}
	}

	if (GetMouseLeftPressed(mHit, mButton, event) && mShapes.size())
	{
		mVertex->UpdateVertex(mCamera->SetWindowCoordinate(mPos.toPoint()));

		// Check snapping
		std::list<Shape*>::iterator iter = mShapes.begin();

		for (iter; iter != mShapes.end(); iter++)
		{
			if ((*iter)->CheckShapeType("Point") && ((*iter) != mVertex))
			{
				mSnapVertex = dynamic_cast<Vertex*>(*iter)->GetVertex();
				mSnapPolygon = SelectUtils::CreatePointBoundingBox(mCamera, &mSnapVertex, 10);
				mSnap = SelectUtils::HitTesting(mPos, mSnapPolygon);

				if (mSnap)
				{
					// Snap이 발생하면 현재 Vertex를 해당 위치로 업데이트
					QPointF p = mSnapVertex.GetVertex();
					mVertex->UpdateVertex(p);
					mSnapPoint = mCamera->SetScreenCoordinate(p);
					break;
				}
			}
		}

		// Snap 발생과 상관없이 Vertex의 마지막 위치에서 bounding box 생성
		mPolygon = SelectUtils::CreatePointBoundingBox(mCamera, mVertex, 10);
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
		painter->drawPoint(mCamera->SetScreenCoordinate(mVertex->GetVertex()));

		// Draw bounding box
		painter->setPen(QPen(Qt::blue, 2));
		painter->drawPolygon(mPolygon);
	}
}
