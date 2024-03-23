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

	QPoint point;
};
