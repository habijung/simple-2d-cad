#pragma once
#include <list>
#include <QPainter>
#include "Camera.h"
using namespace std;


class Shape
{
public:
	virtual ~Shape() {}
	virtual string retType() = 0;
	virtual void render(QPainter* painter, Camera* camera) {}
};

class Vertex : public Shape
{
public:
	Vertex(float x = 0, float y = 0);
	Vertex(QPointF p);
	QPointF retVertex();
	void updateVertex(QPointF p);

	virtual string retType() override;
	virtual void render(QPainter* painter, Camera* camera);

private:
	string mType;
	float mX, mY;
};

class Line : public Shape
{
public:
	Line(Vertex* v1 = nullptr, Vertex* v2 = nullptr);

	virtual string retType() override;
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

	virtual string retType() override;
	virtual void render(QPainter* painter, Camera* camera);

private:
	string mType;
	list<Vertex*> vertices;
};
