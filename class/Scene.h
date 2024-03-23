#pragma once
#include <QWidget>
#include <QPainter>
#include <QSize>
#include <vector>
#include <utility>
using namespace std;


class Scene : public QWidget
{
public:
	Scene(QWidget* parent = nullptr);
	void resize(QSize* screenSize);

	// Coordinate conversion
	void setScreenToWindow(QPoint pScreen, float dx, float dy, float scale);
	QPoint setWindowToScreen(QPointF pWindow);

	// Render method
	void renderOrigin(QPainter* painter);
	void renderScenePoint(QPainter* painter);

private:
	int width, height;
	float dx, dy, scale;
	vector<QPointF> vert;
};
