#include "Camera.h"


Camera::Camera(QWidget* parent) : QWidget(parent)
{
	this->dx = width() / 2.0;
	this->dy = height() / 2.0;
	this->scale = 100.0;
}


// Coordinate conversion
QPointF Camera::setScreenToWindow(QPoint pScreen, float dx, float dy, float scale)
{
	this->dx = dx;
	this->dy = dy;
	this->scale = scale;

	// f({ x, y }) = { (x - dx) / scale, (y - dy) / (-scale) }
	QPointF pWindow =
	{
		(pScreen.x() - dx) / scale,
		(pScreen.y() - dy) / (-scale)
	};
	return pWindow;
}

QPoint Camera::setWindowToScreen(QPointF pWindow)
{
	QPoint pScreen =
	{
		static_cast<int>(pWindow.x() * this->scale + this->dx),
		static_cast<int>(pWindow.y() * (-this->scale) + this->dy)
	};
	return pScreen;
}
