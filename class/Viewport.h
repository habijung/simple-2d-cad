#pragma once
#include <QWidget>
#include <vector>
#include <utility>
#include "Scene.h"
#include "Camera.h"
#include "StateMachine.h"
using namespace std;


class Viewport :
	public QWidget
{
public:
	Viewport(QWidget* parent = nullptr);
	void getKeyEvent(QKeyEvent* event);
	void updateState(string name);
	void saveScene();
	void loadScene();
	void createNewScene();

private:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

	int button;
	float pValue, zValue;
	QPointF mPos;

	// Viewport components
	Scene* mScene;
	Camera* mCamera;
	component* mComp;

	// Define States
	StateMachine* mMachine;
	DrawLineState* mDrawLineState;
	DrawFaceState* mDrawFaceState;
	SelectPointState* mSelectPointState;
	SelectLineState* mSelectLineState;
	SelectFaceState* mSelectFaceState;
};
