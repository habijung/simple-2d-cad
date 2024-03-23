#include "Shape.h"


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

Line::Line(list<Vertex*> vertices)
{
	mType = "Line";
	mV1 = vertices.front();
	mV2 = vertices.back();
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
	QPointF p1 = cam->setScreen(mV1->retVertex());
	QPointF p2 = cam->setScreen(mV2->retVertex());

	return QLineF(p1, p2);
}

void Line::updateLine(Camera* cam, QPointF pStart, QPointF pEnd, vector<Vertex> vec)
{
	QPointF offset = pEnd - pStart;
	QPointF p1 = offset + cam->setScreen(vec.front().retVertex());
	QPointF p2 = offset + cam->setScreen(vec.back().retVertex());
	mV1->updateVertex(cam->setWindow(p1.toPoint()));
	mV2->updateVertex(cam->setWindow(p2.toPoint()));
}

QJsonObject Line::saveLine()
{
	QVariantMap map;
	QPointF v1 = mV1->retVertex();
	QPointF v2 = mV2->retVertex();
	map["v1"] = QString("%1, %2").arg(v1.x()).arg(v1.y());
	map["v2"] = QString("%1, %2").arg(v2.x()).arg(v2.y());

	return QJsonObject::fromVariantMap(map);
}

string Line::type()
{
	return mType;
}

bool Line::checkType(string s)
{
	return !mType.compare(s);
}

void Line::render(QPainter* painter, Camera* camera)
{
	QPointF p1 = camera->setScreen(mV1->retVertex());
	QPointF p2 = camera->setScreen(mV2->retVertex());
	QLineF line = QLineF(p1, p2);
	painter->setPen(QPen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap));
	painter->drawLine(line);
	painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
	painter->drawPoint(p1);
	painter->drawPoint(p2);
}
