#pragma once
#include <QWidget>
#include <vector>
#include <utility>
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
	void selectLine();

	int xOffset, yOffset;
	int button;
	QPoint pStart, pEnd;
	vector<pair<QPoint, QPoint>> vert;
};
