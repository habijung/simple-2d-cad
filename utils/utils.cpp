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

QPolygonF createLineBoundingBox(Camera* cam, Line* l, int d)
{
	QPolygonF poly = QPolygonF();
	QLineF line = l->retLine(cam);
	qreal rad = line.angle() * acos(-1) / 180; // PI = acos(-1)
	qreal dx = d * sin(rad);
	qreal dy = d * cos(rad);
	QPointF dt = QPointF(dx, dy);
	QPointF points[4] =
	{
		line.p1() + dt,
		line.p2() + dt,
		line.p2() - dt,
		line.p1() - dt
	};

	// Counter-Clockwise
	for (int i = 0; i < 4; i++)
	{
		poly << points[i];
	}

	return poly;
}

QPolygonF createLineSelectionBox(Camera* cam, Line* l, int d)
{
	QPolygonF poly = QPolygonF();
	QLineF line = l->retLine(cam);

	// No need to consider polygon direction
	poly << line.p1()
		<< QPointF(line.p1().x(), line.p2().y())
		<< line.p2()
		<< QPointF(line.p2().x(), line.p1().y());

	return poly;
}

bool hitTestingPoint(QPointF pos, QPolygonF poly)
{
	int count = 0;

	for (int i = 0; i < poly.size(); i++)
	{
		// 모든 점과 다음 점의 라인으로 cross check
		int j = (i + 1) % static_cast<int>(poly.size());
		PointReal p1 = { poly[i].x(), poly[i].y() }; // prev
		PointReal p2 = { poly[j].x(), poly[j].y() }; // next

		if (p1.y < pos.y() != p2.y < pos.y())
		{
			qreal x = (p2.x - p1.x) / (p2.y - p1.y) * (pos.y() - p1.y) + p1.x;

			if (pos.x() < x)
			{
				count += 1;
			}
		}
	}

	return (count % 2 == 1 ? true : false);
}
