#pragma once
#include <QWidget>
#include <QResizeEvent>

class Camera
{
public:
	Camera(QSize size = QSize(600, 600), float scale = 100.0);

	// Coordinate conversion
	QPointF setWindow(QPoint pScreen);
	QPoint  setScreen(QPointF pWindow);

	// Camera action
	void zoom(float scale);
	void pan(float dx, float dy);
	void reset(const QSize& size);

	// Camera event
	void resizeEvent(QResizeEvent* event);

private:
	float mX, mY, mScale;
};
