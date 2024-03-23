#pragma once
#include <QWidget>
#include <QPainter>
#include <QSize>
//#include "CentralWidget.h"

class Scene : public QWidget
{
public:
	Scene(QWidget* parent = nullptr);
	void render(QPainter* painter);
	void resize(QSize* screenSize);

private:
	int width, height;
};
