#pragma once
#include <QWidget>
#include <vector>
#include <utility>
#include "Scene.h"
#include "Camera.h"
using namespace std;


class Viewport :
	public QWidget
{
public:
	Viewport(QWidget* parent = nullptr);

private:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

	void drawLine();
	void renderAll();
	void selectLine(QPoint mPoint);
	bool checkSelection(QPoint mPoint, QPolygonF polygon);

	int xOffset, yOffset;
	int button;
	QPoint pStart, pEnd;
	vector<pair<QPoint, QPoint>> vert;

	// Class instances
	Scene* scene;
	Camera* camera;
};
