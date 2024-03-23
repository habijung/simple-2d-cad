#pragma once
#include <QWidget>
#include <QPainter>
#include <QSize>
#include <vector>
#include <utility>
#include <list>
#include "Camera.h"
#include "../shape/Shape.h"

class Scene
{
public:
	Scene(Camera* camera = nullptr);

	std::list<Shape*> retShapes();
	void addShape(Shape* s);
	void updateShapes(std::list<Shape*> newShapes);
	void createSampleShapes();

	// Render method
	void renderCoordinate(QPainter* painter);
	void renderShape(QPainter* painter);

private:
	std::list<Shape*> mShapes;
	Camera* mCamera;
};
