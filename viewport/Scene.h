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

	// Update shape
	std::list<Shape*> retShapes();
	void AddShape(Shape* s);
	void UpdateShapes(std::list<Shape*> newShapes);
	void CreateSampleShapes();

	// Render method
	void RenderCoordinate(QPainter* painter);
	void RenderShape(QPainter* painter);

private:
	std::list<Shape*> mShapes;
	Camera* mCamera;
};
