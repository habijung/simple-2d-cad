#include "Scene.h"

constexpr int SCREEN_LIMIT = 15;

Scene::Scene(Camera* camera)
{
	mCamera = camera;
}

std::list<Shape*> Scene::retShapes()
{
	return mShapes;
}

void Scene::AddShape(Shape* shape)
{
	mShapes.push_back(shape);
}

void Scene::UpdateShapes(const std::list<Shape*>& newShapes)
{
	mShapes = newShapes;
}

void Scene::CreateSampleShapes()
{
	Vertex* v1 = new Vertex(0, 1);
	Vertex* v2 = new Vertex(-1, 2);
	Vertex* v3 = new Vertex(-2, 0);
	Vertex* v4 = new Vertex(1, 2);
	Vertex* v5 = new Vertex(2, -2);

	// Add Face
	std::list<Vertex*> vertices;
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
void Scene::RenderCoordinate(QPainter* painter)
{
	for (int i = -SCREEN_LIMIT; i <= SCREEN_LIMIT; i++)
	{
		for (int j = -SCREEN_LIMIT; j <= SCREEN_LIMIT; j++)
		{
			QLine xAxis = QLine(mCamera->SetScreenCoordinate(QPoint(i, j)), mCamera->SetScreenCoordinate(QPoint(-i, j)));
			QLine yAxis = QLine(mCamera->SetScreenCoordinate(QPoint(i, -j)), mCamera->SetScreenCoordinate(QPoint(i, j)));

			if (i == 0 || j == 0)
			{
				QPoint p = mCamera->SetScreenCoordinate(QPoint(i, j));
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

void Scene::RenderShape(QPainter* painter)
{
	for (std::list<Shape*>::iterator iter = mShapes.begin(); iter != mShapes.end(); iter++)
	{
		(*iter)->Render(painter, mCamera);
	}
}
