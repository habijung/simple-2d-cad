#include "Scene.h"

Scene::Scene(QWidget* parent) : QWidget(parent)
{
	this->width = parent->width();
	this->height = parent->height();
}

void Scene::render(QPainter* painter)
{
	painter->drawText
	(
		this->width / 2,
		this->height / 2,
		QString("%1, %2")
		.arg(this->width / 2)
		.arg(this->height / 2)
	);
}

void Scene::resize(QSize* screenSize)
{
	this->width = screenSize->width();
	this->height = screenSize->height();
}
