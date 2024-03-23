#pragma once
#include <QWidget>
#include <QPainter>
#include <QSize>
#include <vector>
#include <utility>
#include "Camera.h"
using namespace std;


class Scene : public QWidget
{
public:
	Scene(QWidget* parent = nullptr);
	void resize(QSize* screenSize);

	// Render method
	void renderOrigin(QPainter* painter);
	void renderScenePoint(QPainter* painter, Camera* camera);

	// Add data method
	void addVertex(QPointF v);

private:
	int width, height;
	vector<QPointF> vert;
};
