#include <QPainter>
#include <QMouseEvent>
#include "CentralWidget.h"


CentralWidget::CentralWidget(QWidget* parent)
	: QWidget(parent)
{
	qDebug() << "\n Central Widget \n";
	count = 0;
}

void CentralWidget::paintEvent(QPaintEvent* event)
{
	int x = this->point.x();
	int y = this->point.y();
	int offset = 100;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(Qt::black, 10, Qt::DashDotLine, Qt::RoundCap));

	if (x | y)
	{
		painter.drawLine(x, y, x + offset, y + offset);
	}
	painter.end();

	qDebug();
	qDebug() << "x: " << this->point.x();
	qDebug() << "y: " << this->point.y();
}

void CentralWidget::mousePressEvent(QMouseEvent* event)
{
	qDebug() << "\n" << "Mouse Press";
	this->point = event->pos();
	update();
}

void CentralWidget::mouseMoveEvent(QMouseEvent* event)
{
	qDebug() << "Mouse Move: " << count;
	count += 1;
}

void CentralWidget::mouseReleaseEvent(QMouseEvent* event)
{
	qDebug() << "\n" << "Mouse Release";
}
