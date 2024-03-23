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
