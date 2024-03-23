#pragma once
#include <QWidget>
#include <vector>
using namespace std;


class CentralWidget :
	public QWidget
{
public:
	CentralWidget(QWidget* parent = nullptr);

private:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void drawLine();
	void renderAll();

	QPoint pStart, pEnd;
	int xOffset, yOffset;
	vector<vector<QPoint>> vert;
};
