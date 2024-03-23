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

// TODO: Move to Class Camera
void Scene::setScreenToWindow(QPoint pScreen, float dx, float dy, float scale)
{
	this->dx = dx;
	this->dy = dy;
	this->scale = scale;

	// Function:
	// { x, y } = { (x - dx) / scale, (y - dy) / scale }
	QPointF pWindow =
	{
		(pScreen.x() - dx) / scale,
		(pScreen.y() - dy) / scale * -1
	};
	this->vert.push_back(pWindow);
	qDebug() << "Window:" << pWindow;
	qDebug() << "Screen:" << this->setWindowToScreen(this->vert[this->vert.size() - 1]);
}

// TODO: Move to Class Camera
QPoint Scene::setWindowToScreen(QPointF pWindow)
{
	QPoint pScreen =
	{
		static_cast<int>(pWindow.x() * this->scale + this->dx),
		static_cast<int>(pWindow.y() * this->scale * -1 + this->dy)
	};
	return pScreen;
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

void Scene::renderScenePoint(QPainter* painter)
{
	vector<QPointF>::iterator iter = this->vert.begin();
	QPen pen(Qt::black, 5);
	painter->setPen(pen);

	for (iter; iter != this->vert.end(); iter++)
	{
		painter->drawPoint(this->setWindowToScreen(*iter));
	}
}
