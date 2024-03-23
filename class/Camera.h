#pragma once
#include <QWidget>


class Camera : public QWidget
{
public:
	Camera(QWidget* parent = nullptr, QSize size = QSize(600, 600), float scale = 100.0);

	// Coordinate conversion
	QPointF setScreenToWindow(QPoint pScreen, float dx, float dy, float scale);
	QPoint  setWindowToScreen(QPointF pWindow);

private:
	float dx, dy, scale;
};
