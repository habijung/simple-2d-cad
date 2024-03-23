#include "Scene.h"

constexpr int SCREEN_LIMIT = 15;


Scene::Scene(Camera* camera)
{
	mCamera = camera;
	createSampleShapes();
}

list<Shape*> Scene::retShapes()
{
	return mShapes;
}

void Scene::addShape(Shape* s)
{
	mShapes.push_back(s);
}

void Scene::updateShapes(list<Shape*> newShapes)
{
	mShapes = newShapes;
}

void Scene::createSampleShapes()
{
	Vertex* v1 = new Vertex(0, 1);
	Vertex* v2 = new Vertex(-1, 2);
	Vertex* v3 = new Vertex(-2, 0);
	Vertex* v4 = new Vertex(1, 2);
	Vertex* v5 = new Vertex(2, -2);

	// Add Face
	list<Vertex*> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	mShapes.push_back(v1);
	mShapes.push_back(v2);
	mShapes.push_back(v3);
	mShapes.push_back(new Face(vertices));

	// Add Line
	mShapes.push_back(v4);
	mShapes.push_back(v5);
	mShapes.push_back(new Line(v4, v5));
}


// Render method
void Scene::renderCoordinate(QPainter* painter)
{
	for (int i = -SCREEN_LIMIT; i <= SCREEN_LIMIT; i++)
	{
		for (int j = -SCREEN_LIMIT; j <= SCREEN_LIMIT; j++)
		{
			QLine xAxis = QLine(mCamera->setScreen(QPoint(i, j)), mCamera->setScreen(QPoint(-i, j)));
			QLine yAxis = QLine(mCamera->setScreen(QPoint(i, -j)), mCamera->setScreen(QPoint(i, j)));

			if (i == 0 || j == 0)
			{
				QPoint p = mCamera->setScreen(QPoint(i, j));
				painter->setPen(QPen(Qt::black, 0.1));
				painter->setFont(QFont("Arial", 12));

				if (!i && j)
				{
					p += QPoint(5, 7);
					painter->drawText(p, QString("%1").arg(j));
				}
				else if (i && !j)
				{
					p += QPoint(-5, 20);
					painter->drawText(p, QString("%1").arg(i));
				}
				else
				{
					p += QPoint(5, 20);
					painter->drawText(p, QString("%1").arg(i));
				}
			}
			else
			{
				painter->setPen(QPen(Qt::lightGray, 0.05));
			}

			painter->drawLine(xAxis);
			painter->drawLine(yAxis);
		}
	}
}

void Scene::renderShape(QPainter* painter)
{
	list<Shape*>::iterator iter;

	for (iter = this->mShapes.begin(); iter != this->mShapes.end(); iter++)
	{
		(*iter)->render(painter, mCamera);
	}
}
