#include <QPainter>
#include <QMouseEvent>
#include "CentralWidget.h"


CentralWidget::CentralWidget(QWidget* parent)
	: QWidget(parent)
{
	qDebug() << "\n Central Widget \n";
}

void CentralWidget::paintEvent(QPaintEvent* event)
{
	// Draw line with mouse event
	//this->drawLine();

	// Render method test
	//this->renderAll();

	// Selection test
	this->selectLine(this->pStart);
}

void CentralWidget::mousePressEvent(QMouseEvent* event)
{
	this->pStart = event->pos();
	this->button = event->button();
}

void CentralWidget::mouseMoveEvent(QMouseEvent* event)
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

void CentralWidget::mouseReleaseEvent(QMouseEvent* event)
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

void CentralWidget::drawLine()
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

void CentralWidget::renderAll()
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

void CentralWidget::selectLine(QPoint mPoint)
{
	QPointF v1 = { 200, 200 };
	QPointF v2 = { 300, 300 };
	int offset = 20;

	QLineF line(v1, v2);
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

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawLine(v1.x(), v1.y(), v2.x(), v2.y());

	if (this->checkSelection(mPoint, polygon))
	{
		painter.drawPolygon(polygon);
	}

	// Draw mouse click point
	QPen pen;
	pen.setWidth(5);
	pen.setCapStyle(Qt::FlatCap);
	painter.setPen(pen);
	painter.drawPoint(mPoint);
	painter.end();
}

bool CentralWidget::checkSelection(QPoint mPoint, QPolygonF polygon)
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
