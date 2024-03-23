#include "Scene.h"


Scene::Scene(QWidget* parent) : QWidget(parent)
{
	this->width = parent->width();
	this->height = parent->height();
}

void Scene::resize(QSize* screenSize)
{
	this->width = screenSize->width();
	this->height = screenSize->height();
}


// Render method
void Scene::renderOrigin(QPainter* painter)
{
	painter->drawText
	(
		this->width / 2,
		this->height / 2,
		QString("(0, 0)")
	);
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
