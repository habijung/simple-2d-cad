#pragma once
#include <list>
#include <QPainter>
#include "Camera.h"
using namespace std;


class Shape
{
public:
	virtual ~Shape() {}
	virtual void render(QPainter* painter, Camera* camera) {}
};

class Vertex : public Shape
{
public:
	Vertex(float x = 0, float y = 0);
	virtual void render(QPainter* painter, Camera* camera);
	QPointF retVertex();

private:
	string mType;
	float x, y;
};

class Line : public Shape
{
public:
	Line(Vertex* p1 = nullptr, Vertex* p2 = nullptr);
	virtual void render(QPainter* painter, Camera* camera);

private:
	string mType;
	Vertex* p1;
	Vertex* p2;
};

class Face : public Shape
{
public:
	Face(list<Vertex*> vertices = {});
	virtual void render(QPainter* painter, Camera* camera);

private:
	string mType;
	list<Vertex*> vertices;
};
