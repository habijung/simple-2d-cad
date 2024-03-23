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

void Scene::setScreenToWindow(QPoint pScreen, float dx, float dy, float scale)
{
	// Function:
	// { x, y } = { (x - dx) / scale, (y - dy) / scale }
	QPointF pWindow =
	{
		(pScreen.x() - dx) / scale,
		(pScreen.y() - dy) / scale
	};
	this->vert.push_back(pWindow);
	qDebug() << "Window: (" << pWindow.x() << "," << pWindow.y() << ")";
}

QPoint Scene::setWindowToScreen(QPoint p)
{
	return QPoint(p.x() - dx, p.y() - dy);
}

void Scene::renderScreenPoint(QPainter* painter)
{
	vector<QPointF>::iterator iter = this->vert.begin();
	QPen pen(Qt::black, 5);
	painter->setPen(pen);

	for (iter; iter != this->vert.end(); iter++)
	{
		// Change with setWindowToScreen()
		painter->drawPoint(*iter);
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
