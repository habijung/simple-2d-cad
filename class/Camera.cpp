#include "Camera.h"


Camera::Camera(QWidget* parent, QSize size, float scale)
	: QWidget(parent)
{
	this->dx = size.width() / 2.0;
	this->dy = size.height() / 2.0;
	this->scale = scale;
	this->maxWidth = size.width();
	this->maxHeight = size.height();
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


// Camera action
void Camera::zoomIn()
{
	this->scale = this->scale <= 300 ? this->scale + 50 : this->scale;
}

void Camera::zoomOut()
{
	this->scale = this->scale >= 50 ? this->scale - 50 : this->scale;
}

void Camera::pan(float dx, float dy)
{
	// 초기 dx, dy 값이 있기 때문에 반대 방향으로 움직일 때는
	// 그만큼 움직일 거리를 더 줘야함

	// About dx
	this->dx -= dx;

	if (this->dx >= this->maxWidth * 2)
	{
		this->dx = this->maxWidth * 2;
	}
	else if (this->dx <= -this->maxWidth)
	{
		this->dx = -this->maxWidth;
	}

	// About dy
	this->dy += dy;

	if (this->dy >= this->maxHeight * 2)
	{
		this->dy = this->maxHeight * 2;
	}
	else if (this->dy <= -this->maxHeight)
	{
		this->dy = -this->maxHeight;
	}
}
