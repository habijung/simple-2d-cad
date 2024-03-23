#include "Shape.h"


Line::Line(Vertex* v1, Vertex* v2)
{
	mType = "Line";
	mV1 = v1;
	mV2 = v2;
}

Line::Line(const QPointF& p1, const QPointF& p2)
{
	mType = "Line";
	mV1 = new Vertex(p1);
	mV2 = new Vertex(p2);
}

Line::Line(std::list<Vertex*> vertices)
{
	mType = "Line";
	mV1 = vertices.front();
	mV2 = vertices.back();
}

std::vector<Vertex> Line::GetVertices()
{
	std::vector<Vertex> vertices;
	vertices.push_back(mV1->GetVertex());
	vertices.push_back(mV2->GetVertex());

	return vertices;
}

QLineF Line::GetLine(Camera* cam)
{
	QPointF p1 = cam->SetScreenCoordinate(mV1->GetVertex());
	QPointF p2 = cam->SetScreenCoordinate(mV2->GetVertex());

	return QLineF(p1, p2);
}

void Line::UpdateLine(Camera* cam, const QPointF& pStart, const QPointF& pEnd, std::vector<Vertex>& vertices)
{
	QPointF offset = pEnd - pStart;
	QPointF p1 = offset + cam->SetScreenCoordinate(vertices.front().GetVertex());
	QPointF p2 = offset + cam->SetScreenCoordinate(vertices.back().GetVertex());
	mV1->UpdateVertex(cam->SetWindowCoordinate(p1.toPoint()));
	mV2->UpdateVertex(cam->SetWindowCoordinate(p2.toPoint()));
}

QJsonObject Line::SaveLine()
{
	QVariantMap map;
	QPointF v1 = mV1->GetVertex();
	QPointF v2 = mV2->GetVertex();
	map["v1"] = QString("%1, %2").arg(v1.x()).arg(v1.y());
	map["v2"] = QString("%1, %2").arg(v2.x()).arg(v2.y());

	return QJsonObject::fromVariantMap(map);
}

std::string Line::Type()
{
	return mType;
}

bool Line::CheckShapeType(std::string s)
{
	return !mType.compare(s);
}

void Line::Render(QPainter* painter, Camera* camera)
{
	QPointF p1 = camera->SetScreenCoordinate(mV1->GetVertex());
	QPointF p2 = camera->SetScreenCoordinate(mV2->GetVertex());
	QLineF line = QLineF(p1, p2);
	painter->setPen(QPen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap));
	painter->drawLine(line);
	painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
	painter->drawPoint(p1);
	painter->drawPoint(p2);
}
