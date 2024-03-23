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
	Vertex(QPointF p);
	virtual void render(QPainter* painter, Camera* camera);
	QPointF retVertex();

private:
	string mType;
	float mX, mY;
};

class Line : public Shape
{
public:
	Line(Vertex* v1 = nullptr, Vertex* v2 = nullptr);
	virtual void render(QPainter* painter, Camera* camera);

private:
	string mType;
	Vertex* mV1;
	Vertex* mV2;
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
