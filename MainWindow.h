#pragma once

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include "ui_MainWindow.h"
#include "class/Viewport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void setMenuBar();
	void setToolbar();

private:
	Ui::MainWindowClass* ui;

protected:
	//void paintEvent(QPaintEvent* event) override;
};
