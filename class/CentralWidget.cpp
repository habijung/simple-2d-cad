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
	// Draw line with mouse event
	this->drawLine();

	// Render method test
	this->renderAll();
}

void CentralWidget::mousePressEvent(QMouseEvent* event)
{
	this->pStart = event->pos();
	this->button = event->button();
}

void CentralWidget::mouseMoveEvent(QMouseEvent* event)
{
	this->xOffset = event->pos().x() - this->pStart.x();
	this->yOffset = event->pos().y() - this->pStart.y();

	// Draw only mouse left button
	if ((this->button == Qt::LeftButton) &&
		(event->button() == Qt::NoButton))
	{
		update();
	}
}

void CentralWidget::mouseReleaseEvent(QMouseEvent* event)
{
	this->pEnd = event->pos();

	// Draw only mouse left button
	if (this->button == Qt::LeftButton)
	{
		// Add vertex
		pair<QPoint, QPoint> p = { this->pStart, this->pEnd };
		this->vert.push_back(p);
	}
}

void CentralWidget::drawLine()
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

void CentralWidget::renderAll()
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	vector<pair<QPoint, QPoint>>::iterator vIter;

	for (vIter = this->vert.begin(); vIter != this->vert.end(); vIter++)
	{
		QPoint v1 = (*vIter).first;
		QPoint v2 = (*vIter).second;
		painter.drawLine(v1.x(), v1.y(), v2.x(), v2.y());
	}

	painter.end();
}
