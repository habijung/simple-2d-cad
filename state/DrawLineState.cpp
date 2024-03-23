#include "State.h"
#include "../viewport/Viewport.h"

DrawLineState::DrawLineState(std::string name, SelectUtils::ViewportData* data)
	: State(name, data)
{
	mName = name;
	mViewport = data->viewport;
	mScene = data->scene;
	mCamera = data->camera;
	mPoints = {};
	mDrawLine = false;
}

void DrawLineState::UpdateScene(Scene* scene)
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
			Vertex* v1 = new Vertex(mCamera->SetWindowCoordinate(mPoints.front().toPoint()));
			Vertex* v2 = new Vertex(mCamera->SetWindowCoordinate(mPos.toPoint()));
			Line* line = new Line(v1, v2);
			mScene->AddShape(v1);
			mScene->AddShape(v2);
			mScene->AddShape(line);

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
		.arg(mCamera->SetWindowCoordinate(mPos.toPoint()).x(), 0, 'f', 2)
		.arg(mCamera->SetWindowCoordinate(mPos.toPoint()).y(), 0, 'f', 2)
	);
}
