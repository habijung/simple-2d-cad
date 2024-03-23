#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QBoxLayout>
#include <QFileDialog>
#include <QResizeEvent>
#include "ui_MainWindow.h"
#include "viewport/Viewport.h"

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
	void setSidebarWidget(QWidget* widget);
	void setUnderbarWidget(QWidget* widget);

	void resizeEvent(QResizeEvent* event) override;

private:
	Ui::MainWindowClass* ui;
	Viewport* mViewport;
	QWidget* mSidebar, * mUnderbar;
};
