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
	void setScreenToWindow(QPoint p, int xOffset, int yOffset);
	QPoint setWindowToScreen(QPoint p);
	void renderScreen(QPainter* painter);
	void renderWindow(QPainter* painter);

private:
	int width, height;
	int dx, dy;
	vector<QPoint> vert;
};
