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

std::string Vertex::type()
{
	return mType;
}

bool Vertex::checkType(std::string s)
{
	return !mType.compare(s);
}

void Vertex::render(QPainter* painter, Camera* camera)
{
	QPoint p = camera->setScreen(QPointF(mX, mY));
	painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
	painter->drawPoint(p);
}
