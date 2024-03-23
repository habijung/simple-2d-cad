#include <QDebug>
#include "Shape.h"


Vertex::Vertex(float x, float y)
{
	mType = "Point";
	mX = x;
	mY = y;
}

Vertex::Vertex(QPointF p)
{
	mType = "Point";
	mX = p.x();
	mY = p.y();
}

QPointF Vertex::retVertex()
{
	return QPointF(mX, mY);
}

void Vertex::updateVertex(QPointF p)
{
	mX = p.x();
	mY = p.y();
}

string Vertex::retType()
{
	return mType;
}

void Vertex::render(QPainter* painter, Camera* camera)
{
	QPoint p = camera->setWindowToScreen(QPointF(mX, mY));
	painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
	painter->drawPoint(p);
}

Line::Line(Vertex* v1, Vertex* v2)
{
	mType = "Line";
	mV1 = v1;
	mV2 = v2;
}

Line::Line(QPointF p1, QPointF p2)
{
	mType = "Line";
	mV1 = new Vertex(p1);
	mV2 = new Vertex(p2);
}

vector<Vertex> Line::retVertices()
{
	vector<Vertex> vertices;
	vertices.push_back(mV1->retVertex());
	vertices.push_back(mV2->retVertex());

	return vertices;
}

QLineF Line::retLine(Camera* cam)
{
	QPointF p1 = cam->setWindowToScreen(mV1->retVertex());
	QPointF p2 = cam->setWindowToScreen(mV2->retVertex());

	return QLineF(p1, p2);
}

void Line::updateLine(Camera* cam, QPointF pStart, QPointF pEnd, vector<Vertex> vec, float dx, float dy, float scale)
{
	QPointF offset = pEnd - pStart;
	QPointF p1 = offset + cam->setWindowToScreen(vec.front().retVertex());
	QPointF p2 = offset + cam->setWindowToScreen(vec.back().retVertex());
	mV1->updateVertex(cam->setScreenToWindow(p1.toPoint(), dx, dy, scale));
	mV2->updateVertex(cam->setScreenToWindow(p2.toPoint(), dx, dy, scale));
}

string Line::retType()
{
	return mType;
}

void Line::render(QPainter* painter, Camera* camera)
{
	QPointF p1 = camera->setWindowToScreen(mV1->retVertex());
	QPointF p2 = camera->setWindowToScreen(mV2->retVertex());
	QLineF line = QLineF(p1, p2);
	painter->setPen(QPen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap));
	painter->drawLine(line);
	painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
	painter->drawPoint(p1);
	painter->drawPoint(p2);
}

Face::Face(list<Vertex*> vertices)
{
	mType = "Face";
	this->vertices = vertices;
}

string Face::retType()
{
	return mType;
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
