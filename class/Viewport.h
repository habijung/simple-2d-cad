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
	Viewport
	(
		QWidget* parent = nullptr,
		Scene* mainScene = nullptr,
		Camera* mainCamera = nullptr,
		StateMachine* mainMachine = nullptr
	);
	void getKeyEvent(QKeyEvent* event);
	void updateState(State* state, Scene* newScene);
	void saveScene();
	Scene* loadScene(Scene* oldScene);
	Scene* createNewScene(Scene* oldScene);

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

	// Class instances
	Scene* mScene;
	Camera* mCamera;
	StateMachine* mMachine;
	State* mState;
};
