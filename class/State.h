#pragma once

#include <string>
#include <QWidget>
#include "Scene.h"
#include "Camera.h"
using namespace std;

class State
{
public:
	State(string name);
	string getStateName();

	virtual ~State() {}
	virtual void mousePressEvent(QPoint p) {}
	virtual void mouseMoveEvent() {}
	virtual void mouseReleaseEvent() {}

private:
	string mName;
	QWidget* mViewport;
	Scene* mScene;
	Camera* mCamera;
};

class DrawLineState : public State
{
public:
	DrawLineState(string name, QWidget* viewport, Scene* scene, Camera* camera);

	virtual void mousePressEvent(QPoint p);
	virtual void mouseMoveEvent() {}
	virtual void mouseReleaseEvent() {}

private:
	string mName;
	QWidget* mViewport;
	Scene* mScene;
	Camera* mCamera;
};
