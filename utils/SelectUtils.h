#pragma once
#include <QPoint>
#include <QPolygon>
#include "../shape/Shape.h"

class Viewport;
class Scene;
class Camera;

namespace SelectUtils
{
	struct ViewportData
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

	QPolygonF CreatePointBoundingBox(Camera* cam, Vertex* v, int d);
	QPolygonF CreateLineBoundingBox(Camera* cam, Line* l, int d);
	QPolygonF CreateLineSelectionBox(Camera* cam, Line* l);
	QPolygonF CreateFaceSelectionBox(Camera* cam, Face* f);
	bool HitTesting(QPointF pos, QPolygonF poly);
}
