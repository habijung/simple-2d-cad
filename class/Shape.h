#pragma once
#include <list>
#include <QPainter>
#include <QJsonObject>
#include "Camera.h"
using namespace std;


class Shape
{
public:
	virtual ~Shape() {}
	virtual string type() = 0;
	virtual bool checkType(string s) = 0;
	virtual void render(QPainter* painter, Camera* camera) = 0;
};

class Vertex : public Shape
{
public:
	Vertex(float x = 0, float y = 0);
	Vertex(QPointF p);
	QPointF retVertex();
	void updateVertex(QPointF p);

	virtual string type() override;
	virtual bool checkType(string s) override;
	virtual void render(QPainter* painter, Camera* camera) override;

private:
	string mType;
	float mX, mY;
};

class Line : public Shape
{
public:
	Line(Vertex* v1 = nullptr, Vertex* v2 = nullptr);
	Line(QPointF p1, QPointF p2);
	vector<Vertex> retVertices();
	QLineF retLine(Camera* cam);
	void updateLine(Camera* cam, QPointF pStart, QPointF pEnd, vector<Vertex> vStart);
	QJsonObject saveLine();

	virtual string type() override;
	virtual bool checkType(string s) override;
	virtual void render(QPainter* painter, Camera* camera) override;

private:
	string mType;
	Vertex* mV1;
	Vertex* mV2;
};

class Face : public Shape
{
public:
	Face(list<Vertex*> vertices = {});
	Face(Vertex* v);
	vector<Vertex> retVertices();
	QPolygonF retFace(Camera* cam);
	void updateFace(Camera* cam, QPointF pStart, QPointF pEnd, list<Shape*> shapes, vector<Vertex> vertices);
	QJsonObject saveFace();

	virtual string type() override;
	virtual bool checkType(string s) override;
	virtual void render(QPainter* painter, Camera* camera) override;

private:
	string mType;
	list<Vertex*> mVertices;
};
