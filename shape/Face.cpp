#include "Shape.h"


Face::Face(list<Vertex*> vertices)
{
	mType = "Face";
	mVertices = vertices;
}

Face::Face(Vertex* v)
{
	mType = "Face";
	mVertices.push_back(v);
}

vector<Vertex> Face::retVertices()
{
	vector<Vertex> vertices;
	list<Vertex*>::iterator iter = mVertices.begin();

	for (iter; iter != mVertices.end(); iter++)
	{
		vertices.push_back((*iter)->retVertex());
	}

	return vertices;
}

QPolygonF Face::retFace(Camera* cam)
{
	QPolygonF poly;
	list<Vertex*>::iterator iter = mVertices.begin();

	for (iter; iter != mVertices.end(); iter++)
	{
		QPoint p = cam->setScreen((*iter)->retVertex());
		poly << p;
	}

	return poly;
}

void Face::updateFace(Camera* cam, QPointF pStart, QPointF pEnd, list<Shape*> shapes, vector<Vertex> vertices)
{
	QPointF offset = pEnd - pStart;
	list<Shape*>::iterator iter = shapes.begin();

	// 현재 Face*를 먼저 찾은 다음 iteration을 반대로 가면서 Face*의 Vertices를 offset만큼 업데이트
	for (iter; iter != shapes.end(); iter++)
	{
		if ((*iter) == this)
		{
			int count = 0;
			int fSize = vertices.size();

			while (count < fSize)
			{
				count++;
				iter--; // Face* 직전의 연속된 Vertex가 Face*의 Vertices
				Vertex* v = dynamic_cast<Vertex*>(*iter);
				QPointF p = offset + cam->setScreen(vertices[fSize - count].retVertex());
				v->updateVertex(cam->setWindow(p.toPoint()));
			}

			break;
		}
	}
}

QJsonObject Face::saveFace()
{
	QVariantMap map;
	vector<Vertex> vertices = retVertices();

	for (int i = 0; i < vertices.size(); i++)
	{
		string numVertex = "v" + to_string(i + 1);
		QPointF v = vertices[i].retVertex();
		map[numVertex.c_str()] = QString("%1, %2").arg(v.x()).arg(v.y());
	}

	return QJsonObject::fromVariantMap(map);
}

string Face::type()
{
	return mType;
}

bool Face::checkType(string s)
{
	return !mType.compare(s);
}

void Face::render(QPainter* painter, Camera* camera)
{
	QPolygonF poly;
	list<Vertex*>::iterator iter = mVertices.begin();
	vector<QPoint> screenPoints;

	for (iter; iter != mVertices.end(); iter++)
	{
		QPoint p = camera->setScreen((*iter)->retVertex());
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