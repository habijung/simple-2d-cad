#include "State.h"
#include "../viewport/Viewport.h"


DrawFaceState::DrawFaceState(std::string name, viewportData* data)
	: State(name, data)
{
	mName = name;
	mViewport = data->viewport;
	mScene = data->scene;
	mCamera = data->camera;
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
				std::list<Vertex*> vertices;

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
