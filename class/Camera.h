#pragma once
#include <QWidget>


class Camera : public QWidget
{
public:
	Camera(QWidget* parent = nullptr);

	// Coordinate conversion
	QPointF setScreenToWindow(QPoint pScreen, float dx, float dy, float scale);
	QPoint  setWindowToScreen(QPointF pWindow);

private:
	float dx, dy, scale;
};
