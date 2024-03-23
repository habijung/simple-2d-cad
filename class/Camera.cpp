#include "Camera.h"

// Except title, taskbar
constexpr QSize MIN_SIZE = { 400, 300 };


Camera::Camera(QWidget* parent, QSize size, float scale)
	: QWidget(parent)
{
	mX = size.width() / 2.0;
	mY = size.height() / 2.0;
	mScale = scale;
	maxWidth = size.width();
	maxHeight = size.height();
}


// Coordinate conversion
QPointF Camera::setScreenToWindow(QPoint pScreen)
{
	// f({ x, y }) = { (x - mX) / scale, (y - mY) / (-mScale) }
	QPointF pWindow =
	{
		(pScreen.x() - mX) / mScale,
		(pScreen.y() - mY) / (-mScale)
	};
	return pWindow;
}

QPoint Camera::setWindowToScreen(QPointF pWindow)
{
	QPoint pScreen =
	{
		static_cast<int>(pWindow.x() * mScale + mX),
		static_cast<int>(pWindow.y() * (-mScale) + mY)
	};
	return pScreen;
}


// Camera action
void Camera::zoom(float scale)
{
	mScale += scale;

	if (mScale >= 300.0)
	{
		mScale = 300.0;
	}
	else if (mScale <= 50.0)
	{
		mScale = 50.0;
	}
}

void Camera::pan(float dx, float dy)
{
	// 초기 dx, dy 값이 있기 때문에 반대 방향으로 움직일 때는
	// 그만큼 움직일 거리를 더 줘야함

	// About dx
	mX -= dx;

	if (mX >= maxWidth * 2)
	{
		mX = maxWidth * 2;
	}
	else if (mX <= -maxWidth)
	{
		mX = -maxWidth;
	}

	// About dy
	mY += dy;

	if (mY >= maxHeight * 2)
	{
		mY = maxHeight * 2;
	}
	else if (mY <= -maxHeight)
	{
		mY = -maxHeight;
	}
}

void Camera::reset(const QSize& size)
{
	mX = size.width() / 2.0;
	mY = size.height() / 2.0;
	mScale = 100.0;
}

void Camera::resizeEvent(QResizeEvent* event)
{
	if ((event->size().width() >= MIN_SIZE.width()) && (event->size().height() >= MIN_SIZE.height()))
	{
		mX = event->size().width() / 2.0;
		mY = event->size().height() / 2.0;
		mScale = std::min(event->size().width(), event->size().height()) / std::max(event->size().width(), event->size().height());
	}
}
