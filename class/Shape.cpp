#include <QDebug>
#include "Shape.h"


Vertex::Vertex(float x, float y)
{
	mType = "Point";
	this->x = x;
	this->y = y;
}

void Vertex::render(QPainter* painter, Camera* camera)
{
	QPoint p = camera->setWindowToScreen(QPointF(this->x, this->y));
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
	mType = "Line";
	this->p1 = p1;
	this->p2 = p2;
}

void Line::render(QPainter* painter, Camera* camera)
{
	QPointF p1 = camera->setWindowToScreen(this->p1->retVertex());
	QPointF p2 = camera->setWindowToScreen(this->p2->retVertex());
	QLineF line = QLineF(p1, p2);

	QPen pen(Qt::cyan, 6);
	painter->setPen(pen);
	painter->drawLine(line);
}

Face::Face(list<Vertex*> vertices)
{
	mType = "Face";
	this->vertices = vertices;
}

void Face::render(QPainter* painter, Camera* camera)
{
	QPolygonF polygon;
	list<Vertex*>::iterator iter = this->vertices.begin();

	for (iter; iter != this->vertices.end(); iter++)
	{
		polygon << (*iter)->retVertex();
	}

	QPen pen(Qt::black, 3);
	QBrush brush(Qt::green, Qt::SolidPattern);
	painter->setPen(pen);
	painter->setBrush(brush);
	painter->drawPolygon(polygon);
}
