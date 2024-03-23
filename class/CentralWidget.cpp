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
	// TODO: Add attribute with Qt::WA_OpaquePaintEvent

	int x = this->point.x();
	int y = this->point.y();
	int offset = 100;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	if (x | y)
	{
		painter.drawLine(x, y, x + this->xOffset, y + this->yOffset);
		painter.drawText
		(
			this->point.x() + 10,
			this->point.y(),
			QString("x: %1, y: %2")
			.arg(this->point.x())
			.arg(this->point.y())
		);
	}
	painter.end();

	qDebug();
	qDebug() << "x: " << this->point.x();
	qDebug() << "y: " << this->point.y();
}

void CentralWidget::mousePressEvent(QMouseEvent* event)
{
	qDebug() << "Mouse Press";
	this->point = event->pos();
	this->start = event->pos();
}

void CentralWidget::mouseMoveEvent(QMouseEvent* event)
{
	xOffset = event->pos().x() - this->start.x();
	yOffset = event->pos().y() - this->start.y();
	update();
}

void CentralWidget::mouseReleaseEvent(QMouseEvent* event)
{
	qDebug() << "Mouse Release";
}
