#pragma once
#include <list>
#include <QPainter>
#include <QJsonObject>
#include "../viewport/Camera.h"

class Shape
{
public:
	virtual ~Shape() {}
	virtual std::string Type() = 0;
	virtual bool CheckShapeType(const std::string& s) = 0;
	virtual void Render(QPainter* painter, Camera* camera) = 0;
};

class Vertex : public Shape
{
public:
	Vertex(float x = 0, float y = 0);
	Vertex(const QPointF& p);

	QPointF GetVertex();
	void UpdateVertex(const QPointF& p);

	virtual std::string Type() override;
	virtual bool CheckShapeType(const std::string& s) override;
	virtual void Render(QPainter* painter, Camera* camera) override;

private:
	std::string mType;
	float mX, mY;
};

class Line : public Shape
{
public:
	Line(Vertex* v1 = nullptr, Vertex* v2 = nullptr);
	Line(std::list<Vertex*> vertices);

	std::vector<Vertex> GetVertices();
	QLineF GetLine(Camera* cam);
	void UpdateLine(Camera* cam, const QPointF& pStart, const QPointF& pEnd, std::vector<Vertex>& vertices);
	QJsonObject SaveLine();

	virtual std::string Type() override;
	virtual bool CheckShapeType(const std::string& s) override;
	virtual void Render(QPainter* painter, Camera* camera) override;

private:
	std::string mType;
	Vertex* mV1;
	Vertex* mV2;
};

class Face : public Shape
{
public:
	Face(const std::list<Vertex*>& vertices = {});
	Face(Vertex* v);

	std::vector<Vertex> GetVertices();
	QPolygonF GetFace(Camera* cam);
	void UpdateFace(Camera* cam, const QPointF& pStart, const QPointF& pEnd, std::list<Shape*> shapes, std::vector<Vertex>& vertices);
	QJsonObject SaveFace();

	virtual std::string Type() override;
	virtual bool CheckShapeType(const std::string& s) override;
	virtual void Render(QPainter* painter, Camera* camera) override;

private:
	std::string mType;
	std::list<Vertex*> mVertices;
};
