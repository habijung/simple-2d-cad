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
		painter->drawText(pScreen, QString("%1, %2").arg(pWindow.x()).arg(pWindow.y()));
		painter->drawPoint(pScreen);
	}
}

void Scene::renderScenePoint(QPainter* painter, Camera* camera)
{
	vector<QPointF>::iterator iter = this->vert.begin();
	QPen pen(Qt::black, 5);
	painter->setPen(pen);

	for (iter; iter != this->vert.end(); iter++)
	{
		painter->drawPoint(camera->setWindowToScreen(*iter));
	}
}


// Add data method
void Scene::addVertex(QPointF v)
{
	this->vert.push_back(v);
}
