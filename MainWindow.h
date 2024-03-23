#pragma once

#include <QtWidgets/QMainWindow>
#include <QKeyEvent>
#include "ui_MainWindow.h"
#include "class/Viewport.h"
#include "class/StateMachine.h"
#include "utils/utils.h"

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

private:
	Ui::MainWindowClass* ui;
	Viewport* widget;
	Scene* mScene;
	Camera* mCamera;
	component* mData;

	// State Machine
	StateMachine* machine;
	DrawLineState* mDrawLineState;
	DrawFaceState* mDrawFaceState;
	SelectPointState* mSelectPointState;

protected:
	void keyPressEvent(QKeyEvent* event);
};
