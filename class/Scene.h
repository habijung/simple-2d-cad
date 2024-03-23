#pragma once
#include <QWidget>
#include <QPainter>
#include <QSize>
#include <vector>
#include <utility>
#include <list>
#include "Camera.h"
#include "../shape/Shape.h"
using namespace std;


class Scene : public QWidget
{
public:
	Scene(Camera* camera = nullptr);

	list<Shape*> retShapes();
	void addShape(Shape* s);
	void updateShapes(list<Shape*> newShapes);
	void createSampleShapes();

	// Render method
	void renderCoordinate(QPainter* painter);
	void renderShape(QPainter* painter);

private:
	list<Shape*> mShapes;
	Camera* mCamera;
};
