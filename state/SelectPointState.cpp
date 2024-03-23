#include "State.h"
#include "../viewport/Viewport.h"

SelectPointState::SelectPointState(const std::string& name, SelectUtils::ViewportData* data)
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
		for (std::list<Shape*>::reverse_iterator iter = mShapes.rbegin(); iter != mShapes.rend(); iter++)
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
		for (std::list<Shape*>::iterator iter = mShapes.begin(); iter != mShapes.end(); iter++)
		{
			if ((*iter)->CheckShapeType("Point") && ((*iter) != mVertex))
			{
				mSnapVertex = dynamic_cast<Vertex*>(*iter)->GetVertex();
				mSnapPolygon = SelectUtils::CreatePointBoundingBox(mCamera, &mSnapVertex, 10);
				mSnap = SelectUtils::HitTesting(mPos, mSnapPolygon);

				if (mSnap)
				{
					// If occur snap, update Vertex position to another Vertex position
					QPointF p = mSnapVertex.GetVertex();
					mVertex->UpdateVertex(p);
					mSnapPoint = mCamera->SetScreenCoordinate(p);
					break;
				}
			}
		}

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
