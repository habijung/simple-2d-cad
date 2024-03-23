#include <QDebug>
#include "Shape.h"


Vertex::Vertex(float x, float y)
{
	qDebug() << "Create vertex shape";
	this->x = x;
	this->y = y;
}

void Vertex::render(QPainter* painter)
{
	qDebug() << "Draw vertex shape";
}
