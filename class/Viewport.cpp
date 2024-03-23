#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include "Viewport.h"


Viewport::Viewport(QWidget* parent)
	: QWidget(parent)
{
	qDebug() << "\n Central Widget \n";
	scene = new Scene(this);
	camera = new Camera(this, QSize(600, 600), 100.0);
	this->state = new State("NULL");
	this->pValue = 60.0;
	this->zValue = 60.0;
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
	// Activate this when State Machine finished.  
	{
		// Draw line with mouse event
		//this->drawLine();

		// Render method test
		//this->renderAll();

		// Selection test
		//this->selectLine(this->pStart);
	}

	// Class Scene test
	QPainter* painter1 = new QPainter(this);
	scene->renderScreenCoordinate(painter1, this->camera);
	painter1->end();

	QPainter* painter2 = new QPainter(this);
	scene->renderScenePoint(painter2, this->camera);
	painter2->end();

	QPainter* shapePainter = new QPainter(this);
	scene->renderShape(shapePainter);
	shapePainter->end();
}

void Viewport::mousePressEvent(QMouseEvent* event)
{
	this->pStart = event->pos();
	this->button = event->button();

	if (this->button == Qt::RightButton)
	{
		this->xOffset = 0;
		this->yOffset = 0;
		repaint();
	}

	if (this->button == Qt::LeftButton)
	{
		//qDebug() << "Point:" << this->pStart;
		float dx = width() / 2.0;
		float dy = height() / 2.0;
		float scale = 100.0;
		scene->addVertex(camera->setScreenToWindow(this->pStart, dx, dy, scale));
		repaint();

		// Get state and state mouse event test
		this->state->mousePressEvent(this->pStart);
	}
}

void Viewport::mouseMoveEvent(QMouseEvent* event)
{
	this->xOffset = event->pos().x() - this->pStart.x();
	this->yOffset = event->pos().y() - this->pStart.y();

	// Draw only mouse left button
	if ((this->button == Qt::LeftButton) &&
		(event->button() == Qt::NoButton))
	{
		update();
	}
}

void Viewport::mouseReleaseEvent(QMouseEvent* event)
{
	this->pEnd = event->pos();

	// Draw only mouse left button
	if (this->button == Qt::LeftButton)
	{
		// Add vertex
		pair<QPoint, QPoint> p = { this->pStart, this->pEnd };
		this->vert.push_back(p);
	}
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
