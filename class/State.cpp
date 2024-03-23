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
		float dx = mViewport->width() / 2.0;
		float dy = mViewport->height() / 2.0;
		float scale = 100.0;

		Vertex* v = new Vertex(mCamera->setScreenToWindow(mPos.toPoint(), dx, dy, scale));
		mScene->addShape(v);
		mPoints.push_back(mPos);
		mDrawLine = true;

		if (mPoints.size() == 2)
		{
			Vertex* v1 = new Vertex(mCamera->setScreenToWindow(mPoints.front().toPoint(), dx, dy, scale));
			Vertex* v2 = new Vertex(mCamera->setScreenToWindow(mPos.toPoint(), dx, dy, scale));
			Line* line = new Line(v1, v2);
			mScene->addShape(line);

			// Initialization
			mPoints = {};
			mDrawLine = false;
			mButton = Qt::NoButton;
		}
	}

	mViewport->update();
}

void DrawLineState::paintEvent(QPainter* painter)
{
	if (mDrawLine && mPoints.size() != 2)
	{
		painter->setPen(QPen(Qt::darkGray, 3));
		painter->drawLine(mPoints.back(), mPos);
		painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
		painter->drawPoint(mPoints.back());
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

DrawFaceState::DrawFaceState(string name, component* comp)
	: State(name, comp)
{
	mName = name;
	mViewport = comp->viewport;
	mScene = comp->scene;
	mCamera = comp->camera;
}

void DrawFaceState::mousePressEvent(QMouseEvent* event)
{
	qDebug() << "DrawFaceState::mousePressEvent()";
	float dx = this->mViewport->width() / 2.0;
	float dy = this->mViewport->height() / 2.0;
	float scale = 100.0;

	// TODO: Modify addVertex parameter
	//this->mScene->addVertex(this->mCamera->setScreenToWindow(p, dx, dy, scale));
}

void DrawFaceState::paintEvent(QPainter* painter)
{
	QPen pen(Qt::red, 10);
	painter->setPen(pen);
}

SelectPointState::SelectPointState(string name, component* comp)
	: State(name, comp)
{
	mName = name;
	mViewport = comp->viewport;
	mScene = comp->scene;
	mCamera = comp->camera;

	// Test
	v = new Vertex(QPointF(1, 1));
	vs = new Vertex(QPointF(0, 0));
}

void SelectPointState::mousePressEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mButton = event->button();

	pol = QPolygonF();
	pol = createPointBoundingBox(mCamera, v, 25);
	hit = hitTestingPoint(mPos, pol);
	qDebug() << hit;

	// TODO: Scene의 point select
	list<Shape*> shapes = mScene->retShapes();
	list<Shape*>::iterator iter = shapes.begin();

	// TODO: Reverse iterator로 상위 shape 순서 맞추기
	for (iter; iter != shapes.end(); iter++)
	{
		if (!(*iter)->retType().compare("Point"))
		{
			vs = static_cast<Vertex*>(*iter);
			pol = createPointBoundingBox(mCamera, vs, 25);
			hit = hitTestingPoint(mPos, pol);
		}
	}

	mViewport->update();
}

void SelectPointState::mouseMoveEvent(QMouseEvent* event)
{
	// TODO: 마우스 오버 효과 내기
	mPos = event->pos();

	if (hit && mButton == Qt::LeftButton && event->button() == Qt::NoButton)
	{
		float dx = this->mViewport->width() / 2.0;
		float dy = this->mViewport->height() / 2.0;
		float scale = 100.0;

		v->updateVertex(mCamera->setScreenToWindow(mPos.toPoint(), dx, dy, scale));
		pol = createPointBoundingBox(mCamera, v, 25);

		vs->updateVertex(mCamera->setScreenToWindow(mPos.toPoint(), dx, dy, scale));
		pol = createPointBoundingBox(mCamera, v, 25);

		mViewport->update();
	}

	pol = createPointBoundingBox(mCamera, vs, 25);
	hit = hitTestingPoint(mPos, pol);
	mViewport->update();
}

void SelectPointState::mouseReleaseEvent(QMouseEvent* event)
{
	mButton = Qt::NoButton;
}

void SelectPointState::paintEvent(QPainter* painter)
{
	if (mPos.x() > 0 && mPos.y() > 0)
	{
		painter->setPen(QPen(Qt::red, 10));
		painter->drawPoint(mCamera->setWindowToScreen(v->retVertex()));
	}

	if (hit)
	{
		painter->setPen(QPen(Qt::blue, 3));
		painter->drawPolygon(pol);
	}
}
