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
	Scene(Camera* camera = nullptr);
	void updateShapes(list<Shape*> newShapes);

	// Render method
	void renderScreenCoordinate(QPainter* painter);
	void renderShape(QPainter* painter);

	// Add data method
	void addShape(Shape* s);

	// Utility method
	list<Shape*> retShapes();

private:
	vector<QPointF> vertScreen;
	list<Shape*> mShapes;
	Camera* mCamera;
};
