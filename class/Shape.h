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
	Vertex();
	virtual void render(QPainter* painter);
};

class Line : public Shape
{
public:
	Line();
	virtual void render(QPainter* painter);
};

class Face : public Shape
{
public:
	Face();
	virtual void render(QPainter* painter);
};
