#pragma once
#include <QWidget>
#include <QResizeEvent>

class Camera
{
public:
	Camera(QSize size = QSize(600, 600), float scale = 100.0);

	// Coordinate conversion
	QPointF SetWindowCoordinate(QPoint pScreen);
	QPoint  SetScreenCoordinate(QPointF pWindow);

	// Camera action
	void Zoom(float scale);
	void Pan(float dx, float dy);
	void Reset(const QSize& size);

	// Camera event
	void resizeEvent(QResizeEvent* event);

private:
	float mX, mY, mScale;
};
