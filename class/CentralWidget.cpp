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
	this->selectLine();
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

void CentralWidget::selectLine()
{
	QPointF v1 = { 200, 200 };
	QPointF v2 = { 300, 300 };
	int offset = 20;

	QLineF line(v1, v2);
	qreal rad = line.angle() * acos(-1) / 180; // pi = acos(-1)
	qreal dx = offset * sin(rad);
	qreal dy = offset * cos(rad);
	QPointF delta[2] = { QPointF(dx, dy), QPointF(-dx, -dy) };
	QPointF vo[4] =
	{
		line.p1() + delta[0],
		line.p1() + delta[1],
		line.p2() + delta[0],
		line.p2() + delta[1]
	};

	QPolygonF p;
	p << vo[0] << vo[1] << vo[3] << vo[2]; // Clockwise

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawLine(v1.x(), v1.y(), v2.x(), v2.y());
	painter.drawPolygon(p);

	QPointF v = { 185.858, 214.142 };
	QPen pen;
	pen.setWidth(5);
	pen.setCapStyle(Qt::FlatCap);
	painter.setPen(pen);
	painter.drawPoint(v);

	int count = 0;

	for (int i = 0; i < p.size(); i++)
	{
		int j = (i + 1) % static_cast<int>(p.size());

		struct Point
		{
			qreal x, y;
		};
		Point curr = { p[i].x(), p[i].y() };
		Point next = { p[j].x(), p[j].y() };

		// Or, solve it using mixmax() function.
		if (curr.y < v.y() != next.y < v.y())
		{
			// Relation: (x - curr.x) : (v.y() - curr.y) = (next.x - curr.x) : (next.y - curr.y)
			qreal x = (next.x - curr.x) / (next.y - curr.y) * (v.y() - curr.y) + curr.x;

			if (v.x() < x)
			{
				count += 1;
			}
		}
	}

	// Odd: In | Even: Out
	qDebug() << (count % 2 != 0);

	painter.end();
}
