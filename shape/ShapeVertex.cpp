#include "Shape.h"


Vertex::Vertex(float x, float y)
{
	mType = "Point";
	mX = x;
	mY = y;
}

Vertex::Vertex(const QPointF& p)
{
	mType = "Point";
	mX = p.x();
	mY = p.y();
}

QPointF Vertex::GetVertex()
{
	return QPointF(mX, mY);
}

void Vertex::UpdateVertex(const QPointF& p)
{
	mX = p.x();
	mY = p.y();
}

std::string Vertex::Type()
{
	return mType;
}

bool Vertex::CheckShapeType(const std::string& s)
{
	return !mType.compare(s);
}

void Vertex::Render(QPainter* painter, Camera* camera)
{
	QPoint p = camera->SetScreenCoordinate(QPointF(mX, mY));
	painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
	painter->drawPoint(p);
}
