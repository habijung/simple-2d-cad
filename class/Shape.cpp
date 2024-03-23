#include <QDebug>
#include "Shape.h"


Vertex::Vertex(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Vertex::render(QPainter* painter)
{
	QPoint p = QPoint(this->x, this->y);
	QPen pen(Qt::blue, 10);
	painter->setPen(pen);
	painter->drawPoint(p);
}

QPointF Vertex::retVertex()
{
	return QPointF(this->x, this->y);
}

Line::Line(Vertex* p1, Vertex* p2)
{
	this->p1 = p1;
	this->p2 = p2;
}

void Line::render(QPainter* painter)
{
	QPointF p1 = this->p1->retVertex();
	QPointF p2 = this->p2->retVertex();
	QLineF line = QLineF(p1, p2);

	QPen pen(Qt::red, 6);
	pen.setCapStyle(Qt::FlatCap);
	painter->setPen(pen);
	painter->drawLine(line);
}
