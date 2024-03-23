#pragma once
#include <QPainter>


class Shape
{
public:
	virtual ~Shape() {}
	virtual void render(QPainter* painter) {}
};

class Vertex : public Shape
{
public:
	Vertex(float x = 0, float y = 0);
	virtual void render(QPainter* painter);
	QPointF retVertex();

private:
	float x, y;
};

class Line : public Shape
{
public:
	Line(Vertex* p1 = nullptr, Vertex* p2 = nullptr);
	virtual void render(QPainter* painter);

private:
	Vertex* p1;
	Vertex* p2;
};

class Face : public Shape
{
public:
	Face();
	virtual void render(QPainter* painter);
};
