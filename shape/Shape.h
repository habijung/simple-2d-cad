#pragma once
#include <list>
#include <QPainter>
#include <QJsonObject>
#include "../viewport/Camera.h"


class Shape
{
public:
	virtual ~Shape() {}
	virtual std::string type() = 0;
	virtual bool checkType(std::string s) = 0;
	virtual void render(QPainter* painter, Camera* camera) = 0;
};

class Vertex : public Shape
{
public:
	Vertex(float x = 0, float y = 0);
	Vertex(QPointF p);
	QPointF retVertex();
	void updateVertex(QPointF p);

	virtual std::string type() override;
	virtual bool checkType(std::string s) override;
	virtual void render(QPainter* painter, Camera* camera) override;

private:
	std::string mType;
	float mX, mY;
};

class Line : public Shape
{
public:
	Line(Vertex* v1 = nullptr, Vertex* v2 = nullptr);
	Line(QPointF p1, QPointF p2);
	Line(std::list<Vertex*> vertices);
	std::vector<Vertex> retVertices();
	QLineF retLine(Camera* cam);
	void updateLine(Camera* cam, QPointF pStart, QPointF pEnd, std::vector<Vertex> vStart);
	QJsonObject saveLine();

	virtual std::string type() override;
	virtual bool checkType(std::string s) override;
	virtual void render(QPainter* painter, Camera* camera) override;

private:
	std::string mType;
	Vertex* mV1;
	Vertex* mV2;
};

class Face : public Shape
{
public:
	Face(std::list<Vertex*> vertices = {});
	Face(Vertex* v);
	std::vector<Vertex> retVertices();
	QPolygonF retFace(Camera* cam);
	void updateFace(Camera* cam, QPointF pStart, QPointF pEnd, std::list<Shape*> shapes, std::vector<Vertex> vertices);
	QJsonObject saveFace();

	virtual std::string type() override;
	virtual bool checkType(std::string s) override;
	virtual void render(QPainter* painter, Camera* camera) override;

private:
	std::string mType;
	std::list<Vertex*> mVertices;
};
