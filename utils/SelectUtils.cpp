#include "SelectUtils.h"
#include "../viewport/Viewport.h"
#include "../viewport/Scene.h"
#include "../viewport/Camera.h"

QPolygonF SelectUtils::CreatePointBoundingBox(Camera* cam, Vertex* v, int d)
{
	QPolygonF poly = QPolygonF();
	QPointF p = cam->SetScreenCoordinate(v->GetVertex());
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

QPolygonF SelectUtils::CreateLineBoundingBox(Camera* cam, Line* l, int d)
{
	QPolygonF poly = QPolygonF();
	QLineF line = l->GetLine(cam);
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

QPolygonF SelectUtils::CreateLineSelectionBox(Camera* cam, Line* l)
{
	QPolygonF poly = QPolygonF();
	QLineF line = l->GetLine(cam);

	// No need to consider polygon direction
	poly << line.p1()
		<< QPointF(line.p1().x(), line.p2().y())
		<< line.p2()
		<< QPointF(line.p2().x(), line.p1().y());

	return poly;
}

QPolygonF SelectUtils::CreateFaceSelectionBox(Camera* cam, Face* f)
{
	QPolygonF poly = f->GetFace(cam);
	SelectUtils::PointReal min = { INFINITY, INFINITY };
	SelectUtils::PointReal max = { -INFINITY, -INFINITY };

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

bool SelectUtils::HitTesting(const QPointF& pos, QPolygonF poly)
{
	int count = 0;

	for (int i = 0; i < poly.size(); i++)
	{
		// 모든 점과 다음 점의 라인으로 cross check
		int j = (i + 1) % static_cast<int>(poly.size());
		SelectUtils::PointReal p1 = { poly[i].x(), poly[i].y() }; // prev
		SelectUtils::PointReal p2 = { poly[j].x(), poly[j].y() }; // next

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
