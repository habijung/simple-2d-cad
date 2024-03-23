#include "utils.h"
#include "../class/Viewport.h"
#include "../class/Scene.h"
#include "../class/Camera.h"


QPolygonF createPointBoundingBox(Camera* cam, Vertex* v, int d)
{
	QPolygonF poly = QPolygonF();
	QPointF p = cam->setScreen(v->retVertex());
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

QPolygonF createLineSelectionBox(Camera* cam, Line* l)
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

QPolygonF createFaceSelectionBox(Camera* cam, Face* f)
{
	QPolygonF poly = f->retFace(cam);
	PointReal min = { INFINITY, INFINITY };
	PointReal max = { -INFINITY, -INFINITY };

	for (int i = 0; i < poly.size(); i++)
	{
		if (min.x >= poly[i].x())
		{
			min.x = poly[i].x();
		}

		if (min.y >= poly[i].y())
		{
			min.y = poly[i].y();
		}

		if (max.x <= poly[i].x())
		{
			max.x = poly[i].x();
		}

		if (max.y <= poly[i].y())
		{
			max.y = poly[i].y();
		}
	}

	// Counter-Clockwise
	poly = QPolygonF();
	poly << QPointF(max.x, max.y)
		<< QPointF(min.x, max.y)
		<< QPointF(min.x, min.y)
		<< QPointF(max.x, min.y);

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
			// Relation: (x - curr.x) : (v.y() - curr.y) = (next.x - curr.x) : (next.y - curr.y)
			qreal x = (p2.x - p1.x) / (p2.y - p1.y) * (pos.y() - p1.y) + p1.x;

			if (pos.x() < x)
			{
				count += 1;
			}
		}
	}

	// Odd: In | Even: Out
	return (count % 2 == 1 ? true : false);
}
