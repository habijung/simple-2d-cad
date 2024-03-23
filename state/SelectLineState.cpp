#include "State.h"
#include "../viewport/Viewport.h"

SelectLineState::SelectLineState(std::string name, SelectUtils::ViewportData* data)
	: State(name, data)
{
	mName = name;
	mViewport = data->viewport;
	mScene = data->scene;
	mCamera = data->camera;
	mLine = new Line(QPointF(INFINITY, INFINITY), QPointF(INFINITY, INFINITY));
}

void SelectLineState::UpdateScene(Scene* scene)
{
	mScene = scene;
}

void SelectLineState::mousePressEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mPosStart = event->pos();
	mLineVertices = mLine->GetVertices();
	mButton = event->button();
}

void SelectLineState::mouseMoveEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mShapes = mScene->retShapes();

	if (!(mButton == Qt::LeftButton))
	{
		for (std::list<Shape*>::reverse_iterator iter = mShapes.rbegin(); iter != mShapes.rend(); iter++)
		{
			if ((*iter)->CheckShapeType("Line"))
			{
				mLine = static_cast<Line*>(*iter);
				mPolygon = SelectUtils::CreateLineBoundingBox(mCamera, mLine, 10);
				mHit = SelectUtils::HitTesting(mPos, mPolygon);

				if (mHit)
				{
					break;
				}
			}
		}
	}

	if (GetMouseLeftPressed(mHit, mButton, event))
	{
		mLine->UpdateLine(mCamera, mPosStart, mPos, mLineVertices);
		mPolygon = SelectUtils::CreateLineBoundingBox(mCamera, mLine, 10);
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
		painter->drawLine(mLine->GetLine(mCamera));

		// Draw bounding box
		mPolygon = SelectUtils::CreateLineBoundingBox(mCamera, mLine, 10);
		painter->setPen(QPen(Qt::blue, 3));
		painter->drawPolygon(mPolygon);

		// Draw selection box
		mPolygon = SelectUtils::CreateLineSelectionBox(mCamera, mLine);
		painter->setPen(QPen(Qt::green, 3));
		painter->drawPolygon(mPolygon);
	}
}
