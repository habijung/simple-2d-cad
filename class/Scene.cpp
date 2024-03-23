#include "Scene.h"


Scene::Scene(QWidget* parent, Camera* camera) : QWidget(parent)
{
	this->width = parent->width();
	this->height = parent->height();
	mCamera = camera;

	for (int i = -10; i <= 10; i++)
	{
		for (int j = -10; j <= 10; j++)
		{
			this->vertScreen.push_back(QPointF(i, j));
		}
	}

	// Sample Shape
	{
		Vertex* v1 = new Vertex(150, 150);
		Vertex* v2 = new Vertex(350, 350);
		Vertex* v3 = new Vertex(300, 200);
		mShapes.push_back(v1);
		mShapes.push_back(v2);
		mShapes.push_back(v3);

		list<Vertex*> vertices;
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		Face* f1 = new Face(vertices);
		mShapes.push_back(f1);

		Vertex* v4 = new Vertex(1, 2);
		Vertex* v5 = new Vertex(2, -2);
		Line* l1 = new Line(v4, v5);
		mShapes.push_back(v4);
		mShapes.push_back(v5);
		mShapes.push_back(l1);
	}
}

void Scene::resize(QSize* screenSize)
{
	this->width = screenSize->width();
	this->height = screenSize->height();
}


// Render method
void Scene::renderScreenCoordinate(QPainter* painter)
{
	vector<QPointF>::iterator iter = this->vertScreen.begin();
	QPen pen(Qt::black, 2);
	painter->setPen(pen);

	for (iter; iter != this->vertScreen.end(); iter++)
	{
		QPointF pWindow = *iter;
		QPoint  pScreen = mCamera->setWindowToScreen(pWindow);
		painter->drawPoint(pScreen);

		if (pWindow.x() == 0 || pWindow.y() == 0)
		{
			if (pWindow.x() == 0)
			{
				painter->drawText(pScreen, QString("%1").arg(pWindow.y()));
			}
			else
			{
				painter->drawText(pScreen, QString("%1").arg(pWindow.x()));
			}
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


// Add data method
void Scene::addShape(Shape* s)
{
	mShapes.push_back(s);
}


// Utility method
list<Shape*> Scene::retShapes()
{
	return mShapes;
}
