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
	void setScreenToWindow(QPoint p, int dx, int dy);

private:
	int width, height;
	vector<QPoint> vert;
};
