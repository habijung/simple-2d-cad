#include <QPainter>
#include "CentralWidget.h"


CentralWidget::CentralWidget(QWidget* parent)
	: QWidget(parent)
{
	qDebug() << "\n Central Widget \n";
}

void CentralWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(Qt::black, 10, Qt::DashDotLine, Qt::RoundCap));
	painter.drawLine(100, 100, 200, 200);
	painter.end();
}
