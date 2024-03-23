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

void Scene::setScreenToWindow(QPoint p, int xOffset, int yOffset)
{
	this->dx = xOffset;
	this->dy = yOffset;

	QPoint newPoint = { p.x() + dx, p.y() + dy };
	this->vert.push_back(newPoint);
}

QPoint Scene::setWindowToScreen(QPoint p)
{
	return QPoint(p.x() - dx, p.y() - dy);
}

void Scene::renderScreen(QPainter* painter)
{
	QPen pen(Qt::black, 5);
	painter->setPen(pen);

	for (int i = 0; i < this->vert.size(); i++)
	{
		painter->drawPoint(this->setWindowToScreen(this->vert[i]));
	}
}

void Scene::renderWindow(QPainter* painter)
{
	QPen pen(Qt::red, 5);
	painter->setPen(pen);

	for (int i = 0; i < this->vert.size(); i++)
	{
		painter->drawPoint(this->vert[i]);
	}
}
