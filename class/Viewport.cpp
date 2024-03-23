#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include "Viewport.h"


Viewport::Viewport(QWidget* parent, Scene* mainScene, Camera* mainCamera, StateMachine* mainMachine)
	: QWidget(parent)
{
	qDebug() << "\n Central Widget \n";
	scene = mainScene;
	camera = mainCamera;
	machine = mainMachine;
	component* comp = new component;
	this->state = new SelectPointState("NULL", comp);
	this->pValue = 80.0;
	this->zValue = 60.0;
	this->setMouseTracking(true);
}

void Viewport::getKeyEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_E: // Zoom In
		this->camera->zoom(this->zValue);
		break;
	case Qt::Key_Q: // Zoom Out
		this->camera->zoom(-this->zValue);
		break;
	case Qt::Key_W: // Move Up
		this->camera->pan(0.0, this->pValue);
		break;
	case Qt::Key_S: // Move Down
		this->camera->pan(0.0, -this->pValue);
		break;
	case Qt::Key_D: // Move Right
		this->camera->pan(this->pValue, 0.0);
		break;
	case Qt::Key_A: // Move Left
		this->camera->pan(-this->pValue, 0.0);
		break;
	}
	repaint();
}

void Viewport::updateState(State* state)
{
	this->state = state;
}

void Viewport::paintEvent(QPaintEvent* event)
{
	QPainter* painter = new QPainter(this);

	painter->save();
	scene->renderScreenCoordinate(painter);
	scene->renderShape(painter);
	painter->restore();

	painter->save();
	machine->getCurrentState()->paintEvent(painter);
	painter->restore();

	// Activate this when State Machine finished.  
	{
		// Draw line with mouse event
		//this->drawLine();

		// Render method test
		//this->renderAll();

		// Selection test
		//this->selectLine(this->pStart);
	}

	painter->end();
}

void Viewport::mousePressEvent(QMouseEvent* event)
{
	mPos = event->pos();
	button = event->button();
	state->mousePressEvent(event);
}

void Viewport::mouseMoveEvent(QMouseEvent* event)
{
	if ((button == Qt::RightButton) && (event->button() == Qt::NoButton))
	{
		QPointF offset = event->pos() - mPos;
		this->camera->pan(-offset.x(), offset.y());
		mPos = event->pos();
	}

	state->mouseMoveEvent(event);
}

void Viewport::mouseReleaseEvent(QMouseEvent* event)
{
	button = Qt::NoButton;
	state->mouseReleaseEvent(event);
}

void Viewport::wheelEvent(QWheelEvent* event)
{
	camera->zoom(static_cast<float>(event->angleDelta().y() / 8));
	update();
}

void Viewport::resizeEvent(QResizeEvent* event)
{
	// TODO: resizeEvent가 발생하면 좌표를 실시간으로 이동
	QSize* screenSize = new QSize(width(), height());
	scene->resize(screenSize);
}

void Viewport::drawLine()
{
	// TODO: Add attribute with Qt::WA_OpaquePaintEvent
	int xStart = this->pStart.x();
	int yStart = this->pStart.y();
	int xEnd = xStart + this->xOffset;
	int yEnd = yStart + this->yOffset;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	if (xStart | yStart)
	{
		painter.drawLine(xStart, yStart, xEnd, yEnd);
		painter.drawText
		(
			xStart + 10,
			yStart,
			QString("x: %1, y: %2")
			.arg(xStart)
			.arg(yStart)
		);
		painter.drawText
		(
			xEnd + 10,
			yEnd,
			QString("x: %1, y: %2")
			.arg(xEnd)
			.arg(yEnd)
		);
	}
	painter.end();
}

void Viewport::renderAll()
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	vector<pair<QPoint, QPoint>>::iterator vIter;

	for (vIter = this->vert.begin(); vIter != this->vert.end(); vIter++)
	{
		QPoint v1 = (*vIter).first;
		QPoint v2 = (*vIter).second;
		painter.drawLine(v1.x(), v1.y(), v2.x(), v2.y());
	}

	painter.end();
}

void Viewport::selectLine(QPoint mPoint)
{
	int offset = 10; // Line bounding box width offset
	QPainter painter(this);
	QPointF p1, p2;
	QLineF line;
	vector<pair<QPoint, QPoint>>::reverse_iterator vrIter;

	for (vrIter = this->vert.rbegin(); vrIter != this->vert.rend(); vrIter++)
	{
		p1 = (*vrIter).first;
		p2 = (*vrIter).second;
		line = QLineF(p1, p2);
		qreal rad = line.angle() * acos(-1) / 180; // pi = acos(-1)
		qreal dx = offset * sin(rad);
		qreal dy = offset * cos(rad);
		QPointF delta[2] = { QPointF(dx, dy), QPointF(-dx, -dy) };
		QPointF pOffset[4] =
		{
			line.p1() + delta[0],
			line.p1() + delta[1],
			line.p2() + delta[0],
			line.p2() + delta[1]
		};

		QPolygonF polygon;
		polygon << pOffset[0] << pOffset[1] << pOffset[3] << pOffset[2]; // Clockwise

		painter.setRenderHint(QPainter::Antialiasing);
		painter.drawLine(p1.x(), p1.y(), p2.x(), p2.y());

		if (this->checkSelection(mPoint, polygon))
		{
			QPen pen;
			pen.setWidth(2);
			pen.setColor(Qt::blue);
			painter.setPen(pen);
			painter.drawPolygon(polygon);
			break;
		}
	}

	// Draw mouse click point
	QPen pen;
	pen.setWidth(5);
	pen.setCapStyle(Qt::FlatCap);
	pen.setColor(Qt::red);
	painter.setPen(pen);

	if (mPoint != QPoint(0, 0))
	{
		painter.drawPoint(mPoint);
	}

	painter.end();
}

bool Viewport::checkSelection(QPoint mPoint, QPolygonF polygon)
{
	int count = 0;

	for (int i = 0; i < polygon.size(); i++)
	{
		int j = (i + 1) % static_cast<int>(polygon.size());

		struct Point
		{
			qreal x, y;
		};
		Point curr = { polygon[i].x(), polygon[i].y() };
		Point next = { polygon[j].x(), polygon[j].y() };

		// Or, solve it using mixmax() function.
		if (curr.y < mPoint.y() != next.y < mPoint.y())
		{
			// Relation: (x - curr.x) : (v.y() - curr.y) = (next.x - curr.x) : (next.y - curr.y)
			qreal x = (next.x - curr.x) / (next.y - curr.y) * (mPoint.y() - curr.y) + curr.x;

			if (mPoint.x() < x)
			{
				count += 1;
			}
		}
	}

	// Odd: In | Even: Out
	return count % 2 != 0;
}
