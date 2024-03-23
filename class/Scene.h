#pragma once
#include <QWidget>
#include <QPainter>
#include <QSize>
#include <vector>
#include <utility>
#include <list>
#include "Camera.h"
#include "Shape.h"
using namespace std;


class Scene : public QWidget
{
public:
	Scene(QWidget* parent = nullptr);
	void resize(QSize* screenSize);

	// Render method
	void renderScreenCoordinate(QPainter* painter, Camera* camera);
	void renderScenePoint(QPainter* painter, Camera* camera);
	void renderShape(QPainter* painter);

	// Add data method
	void addVertex(QPointF v);

private:
	int width, height;
	vector<QPointF> vertScreen;
	vector<QPointF> vert;
	list<Shape*> mShapes;
};
