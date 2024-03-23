#include "Camera.h"

// Except title, toolbar
constexpr QSize MIN_SIZE = { 400, 300 };
constexpr float TOLERANCE[2] = { -20.0, 21.0 };
constexpr float SCREEN_LIMIT = 15.5;


Camera::Camera(QWidget* parent, QSize size, float scale)
	: QWidget(parent)
{
	mX = size.width() / 2.0 + TOLERANCE[0];
	mY = size.height() / 2.0 + TOLERANCE[1];
	mScale = scale;
}


// Coordinate conversion
QPointF Camera::setWindow(QPoint pScreen)
{
	// f({ x, y }) = { (x - mX) / scale, (y - mY) / (-mScale) }
	QPointF pWindow =
	{
		(pScreen.x() - mX) / mScale,
		(pScreen.y() - mY) / (-mScale)
	};
	return pWindow;
}

QPoint Camera::setScreen(QPointF pWindow)
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

	if (mScale >= 250.0)
	{
		mScale = 250.0;
	}
	else if (mScale <= 40.0)
	{
		mScale = 40.0;
	}

	pan(0, 0);
}

void Camera::pan(float dx, float dy)
{
	float camX = -mX / mScale + dx;
	float camY = mY / mScale + dy;
	float scrLength = 6.0 * 100 / mScale;

	// Calculate x-axis
	// camX 값은 Window의 좌측
	if (camX + scrLength >= SCREEN_LIMIT)
	{
		camX = SCREEN_LIMIT - scrLength;
	}
	else if (camX <= -SCREEN_LIMIT)
	{
		camX = -SCREEN_LIMIT;
	}

	mX = -camX * mScale;

	// Calculate y-axis
	// y-axis는 아래의 footer 때문에 값을 조정함
	if (camY >= SCREEN_LIMIT)
	{
		camY = SCREEN_LIMIT;
	}
	else if (camY - scrLength <= -(SCREEN_LIMIT + 1.0))
	{
		camY = -(SCREEN_LIMIT + 1.0) + scrLength;
	}

	mY = camY * mScale;
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
