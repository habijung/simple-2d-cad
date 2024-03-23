#pragma once
#include <QPoint>
#include <QPolygon>
#include "../class/Shape.h"


// Circular Dependency
class Viewport;
class Scene;
class Camera;

struct component
{
	Viewport* viewport;
	Scene* scene;
	Camera* camera;
};

struct Point
{
	int x;
	int y;
};

struct PointReal
{
	qreal x;
	qreal y;
};

QPolygonF createPointBoundingBox(Camera* cam, Vertex* v, int d);
QPolygonF createLineBoundingBox(Camera* cam, Line* l, int d);
QPolygonF createLineSelectionBox(Camera* cam, Line* l);
QPolygonF createFaceSelectionBox(Camera* cam, Face* f);
bool hitTestingPoint(QPointF pos, QPolygonF poly);
