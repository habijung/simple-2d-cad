#include "Scene.h"


Scene::Scene(QWidget* parent) : QWidget(parent)
{
	this->width = parent->width();
	this->height = parent->height();

	pair<QPoint, QPoint> p = { QPoint(100, 100), QPoint(200, 200) };
	this->vert.push_back(p);
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

	QLineF line = QLineF(this->vert[0].first, this->vert[0].second);
	painter->drawLine(line);
}

void Scene::resize(QSize* screenSize)
{
	this->width = screenSize->width();
	this->height = screenSize->height();
}
