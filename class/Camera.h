#pragma once
#include <QWidget>


class Camera : public QWidget
{
public:
	Camera(QWidget* parent = nullptr, QSize size = QSize(600, 600), float scale = 100.0);

	// Coordinate conversion
	QPointF setScreenToWindow(QPoint pScreen);
	QPoint  setWindowToScreen(QPointF pWindow);

	// Camera action
	void zoom(float scale);
	void pan(float dx, float dy);

private:
	float dx, dy, scale;
	int maxWidth, maxHeight;
};
