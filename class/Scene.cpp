#include "Scene.h"


Scene::Scene(QWidget* parent) : QWidget(parent)
{
	this->width = parent->width();
	this->height = parent->height();

	for (int i = -10; i <= 10; i++)
	{
		for (int j = -10; j <= 10; j++)
		{
			this->vertScreen.push_back(QPointF(i, j));
		}
	}
}

void Scene::resize(QSize* screenSize)
{
	this->width = screenSize->width();
	this->height = screenSize->height();
}


// Render method
void Scene::renderScreenCoordinate(QPainter* painter, Camera* camera)
{
	vector<QPointF>::iterator iter = this->vertScreen.begin();
	QPen pen(Qt::black, 2);
	painter->setPen(pen);

	for (iter; iter != this->vertScreen.end(); iter++)
	{
		QPointF pWindow = *iter;
		QPoint  pScreen = camera->setWindowToScreen(pWindow);
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

void Scene::renderScenePoint(QPainter* painter, Camera* camera)
{
	vector<QPointF>::iterator iter = this->vert.begin();

	for (iter; iter != this->vert.end(); iter++)
	{
		painter->drawPoint(camera->setWindowToScreen(*iter));
	}
}

void Scene::renderShape(QPainter* painter)
{
	// Vertex test
	Vertex* v1 = new Vertex(150, 150);
	Vertex* v2 = new Vertex(350, 350);
	Vertex* v3 = new Vertex(300, 200);
	this->mShapes.push_back(v1);
	this->mShapes.push_back(v2);
	this->mShapes.push_back(v3);

	// Face test
	list<Vertex*> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	Face* f1 = new Face(vertices);
	this->mShapes.push_back(f1);

	// Line test
	Vertex* v4 = new Vertex(400, 150);
	Vertex* v5 = new Vertex(370, 200);
	Line* l1 = new Line(v4, v5);
	this->mShapes.push_back(v4);
	this->mShapes.push_back(v5);
	this->mShapes.push_back(l1);

	list<Shape*>::iterator iter;

	for (iter = this->mShapes.begin(); iter != this->mShapes.end(); iter++)
	{
		(*iter)->render(painter);
	}
}

// Add data method
void Scene::addVertex(QPointF v)
{
	this->vert.push_back(v);
}
