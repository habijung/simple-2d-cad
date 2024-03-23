#include <QDebug>
#include "Shape.h"


Vertex::Vertex()
{
	qDebug() << "Create vertex shape";
}

void Vertex::render(QPainter* painter)
{
	qDebug() << "Draw vertex shape";
}
