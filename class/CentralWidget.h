#pragma once
#include <qwidget.h>


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

	QPoint pStart, pEnd;
	int xOffset, yOffset;
};
