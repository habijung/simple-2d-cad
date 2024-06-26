#include "Shape.h"

Face::Face(const std::list<Vertex*>& vertices)
{
	mType = "Face";
	mVertices = vertices;
}

Face::Face(Vertex* v)
{
	mType = "Face";
	mVertices.push_back(v);
}

std::vector<Vertex> Face::GetVertices()
{
	std::vector<Vertex> vertices;

	for (std::list<Vertex*>::iterator iter = mVertices.begin(); iter != mVertices.end(); iter++)
	{
		vertices.push_back((*iter)->GetVertex());
	}

	return vertices;
}

QPolygonF Face::GetFace(Camera* cam)
{
	QPolygonF poly;

	for (std::list<Vertex*>::iterator iter = mVertices.begin(); iter != mVertices.end(); iter++)
	{
		QPoint p = cam->SetScreenCoordinate((*iter)->GetVertex());
		poly << p;
	}

	return poly;
}

void Face::UpdateFace(Camera* cam, const QPointF& pStart, const QPointF& pEnd, std::list<Shape*> shapes, std::vector<Vertex>& vertices)
{
	QPointF offset = pEnd - pStart;

	// Find current Face* and get vertices as Face size using reverse iterator
	// And update vertex with offset
	for (std::list<Shape*>::iterator iter = shapes.begin(); iter != shapes.end(); iter++)
	{
		if ((*iter) == this)
		{
			int count = 0;
			int fSize = vertices.size();

			while (count < fSize)
			{
				count++;
				iter--; // Continuous vertices just before Face* are Face Vertices
				QPointF p = offset + cam->SetScreenCoordinate(vertices[fSize - count].GetVertex());
				dynamic_cast<Vertex*>(*iter)->UpdateVertex(cam->SetWindowCoordinate(p.toPoint()));
			}

			break;
		}
	}
}

QJsonObject Face::SaveFace()
{
	QVariantMap map;
	std::vector<Vertex> vertices = GetVertices();

	for (int i = 0; i < vertices.size(); i++)
	{
		std::string numVertex = "v" + std::to_string(i + 1);
		QPointF v = vertices[i].GetVertex();
		map[numVertex.c_str()] = QString("%1, %2").arg(v.x()).arg(v.y());
	}

	return QJsonObject::fromVariantMap(map);
}

std::string Face::Type()
{
	return mType;
}

bool Face::CheckShapeType(const std::string& s)
{
	return !mType.compare(s);
}

void Face::Render(QPainter* painter, Camera* camera)
{
	QPolygonF poly;
	std::vector<QPoint> screenPoints;

	for (std::list<Vertex*>::iterator iter = mVertices.begin(); iter != mVertices.end(); iter++)
	{
		QPoint p = camera->SetScreenCoordinate((*iter)->GetVertex());
		poly << p;
		screenPoints.push_back(p);
	}

	// Draw polygon
	painter->setPen(QPen(Qt::darkGray, 3));
	painter->setBrush(QBrush(Qt::cyan));
	painter->drawPolygon(poly, Qt::OddEvenFill);

	// Draw polygon points again for priority
	for (int i = 0; i < screenPoints.size(); i++)
	{
		painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
		painter->drawPoint(screenPoints[i]);
	}
}
