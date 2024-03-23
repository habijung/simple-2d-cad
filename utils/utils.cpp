#include "utils.h"
#include "../class/Viewport.h"
#include "../class/Scene.h"
#include "../class/Camera.h"


QPolygonF createPointBoundingBox(Camera* cam, Vertex* v, int d)
{
	QPolygonF poly = QPolygonF();
	QPointF p = cam->setWindowToScreen(v->retVertex());
	QPointF dt[2] = { QPointF(d, d), QPointF(-d, d) };
	QPointF points[4] =
	{
		p + dt[0],
		p + dt[1],
		p - dt[0],
		p - dt[1]
	};

	// Counter-Clockwise
	for (int i = 0; i < 4; i++)
	{
		poly << points[i];
	}

	return poly;
}
