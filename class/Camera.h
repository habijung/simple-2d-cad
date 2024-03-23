#pragma once
#include <QWidget>
#include <QResizeEvent>


class Camera : public QWidget
{
public:
	Camera(QWidget* parent = nullptr, QSize size = QSize(600, 600), float scale = 100.0);

	// Coordinate conversion
	QPointF setWindow(QPoint pScreen);
	QPoint  setScreen(QPointF pWindow);

	// Camera action
	void zoom(float scale);
	void pan(float dx, float dy);
	void reset(const QSize &size);

	void resizeEvent(QResizeEvent* event) override;

private:
	float mX, mY, mScale;
	int maxWidth, maxHeight;
};
