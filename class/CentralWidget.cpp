#include <QPainter>
#include <QMouseEvent>
#include "CentralWidget.h"


CentralWidget::CentralWidget(QWidget* parent)
	: QWidget(parent)
{
	qDebug() << "\n Central Widget \n";
}

void CentralWidget::paintEvent(QPaintEvent* event)
{
	// TODO: Add attribute with Qt::WA_OpaquePaintEvent
	int xStart = this->pStart.x();
	int yStart = this->pStart.y();
	int xEnd = xStart + this->xOffset;
	int yEnd = yStart + this->yOffset;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	if (xStart | yStart)
	{
		painter.drawLine(xStart, yStart, xEnd, yEnd);
		painter.drawText
		(
			xStart + 10,
			yStart,
			QString("x: %1, y: %2")
			.arg(xStart)
			.arg(yStart)
		);
		painter.drawText
		(
			xEnd + 10,
			yEnd,
			QString("x: %1, y: %2")
			.arg(xEnd)
			.arg(yEnd)
		);
	}
	painter.end();
}

void CentralWidget::mousePressEvent(QMouseEvent* event)
{
	this->pStart = event->pos();
}

void CentralWidget::mouseMoveEvent(QMouseEvent* event)
{
	xOffset = event->pos().x() - this->pStart.x();
	yOffset = event->pos().y() - this->pStart.y();
	update();
}

void CentralWidget::mouseReleaseEvent(QMouseEvent* event)
{
	this->pEnd = event->pos();
}