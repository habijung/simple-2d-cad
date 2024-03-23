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
	void render(QPainter* painter);
	void resize(QSize* screenSize);

	void setScreenToWindow(QPoint pScreen, float dx, float dy, float scale);
	void renderScreenPoint(QPainter* painter);

	// Not ready
	QPoint setWindowToScreen(QPoint p);
	void renderWindow(QPainter* painter);

private:
	int width, height;
	int dx, dy;
	vector<QPointF> vert;
};
