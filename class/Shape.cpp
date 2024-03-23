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
	mVertices = vertices;
}

Face::Face(Vertex* v)
{
	mType = "Face";
	mVertices.push_back(v);
}

vector<Vertex> Face::retVertices()
{
	vector<Vertex> vertices;
	list<Vertex*>::iterator iter = mVertices.begin();

	for (iter; iter != mVertices.end(); iter++)
	{
		vertices.push_back((*iter)->retVertex());
	}

	return vertices;
}

QPolygonF Face::retFace(Camera* cam)
{
	QPolygonF poly;
	list<Vertex*>::iterator iter = mVertices.begin();

	for (iter; iter != mVertices.end(); iter++)
	{
		QPoint p = cam->setWindowToScreen((*iter)->retVertex());
		poly << p;
	}

	return poly;
}

void Face::updateFace(Camera* cam, QPointF pStart, QPointF pEnd, vector<Vertex> vec, float dx, float dy, float scale)
{
	list<Vertex*> newVertices;
	QPointF offset = pEnd - pStart;
	vector<Vertex>::iterator iter = vec.begin();

	for (iter; iter != vec.end(); iter++)
	{
		QPointF p = offset + cam->setWindowToScreen((*iter).retVertex());
		Vertex* v = new Vertex(cam->setScreenToWindow(p.toPoint(), dx, dy, scale));
		newVertices.push_back(v);
	}

	mVertices = newVertices;
}

string Face::retType()
{
	return mType;
}

void Face::render(QPainter* painter, Camera* camera)
{
	QPolygonF poly;
	list<Vertex*>::iterator iter = mVertices.begin();
	vector<QPoint> screenPoints;

	for (iter; iter != mVertices.end(); iter++)
	{
		QPoint p = camera->setWindowToScreen((*iter)->retVertex());
		poly << p;
		screenPoints.push_back(p);
	}

	// Draw polygon
	painter->setPen(QPen(Qt::darkGray, 3));
	painter->setBrush(QBrush(Qt::cyan));
	painter->drawPolygon(poly, Qt::OddEvenFill);

	// Draw polygon points again for priority
	for (int i = 0; i < screenPoints.size(); i++)
	{
		painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
		painter->drawPoint(screenPoints[i]);
	}
}
