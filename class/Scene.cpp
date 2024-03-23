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
		QString("(0, 0)")
	);
}

void Scene::resize(QSize* screenSize)
{
	this->width = screenSize->width();
	this->height = screenSize->height();
}

void Scene::setScreenToWindow(QPoint p, int dx, int dy)
{
	QPoint pp = { p.x() + dx, p.y() + dy};
	
	this->vert.push_back(pp);

	qDebug() << pp << ", " << vert.size();
}
