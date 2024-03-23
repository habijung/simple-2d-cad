#pragma once

#include <string>
#include <QWidget>
#include "Scene.h"
#include "Camera.h"
#include "../utils/utils.h"
using namespace std;


class Viewport; // Circular Dependency

class State
{
public:
	State(string name);
	string getStateName();

	virtual ~State() {}
	virtual void mousePressEvent(QPoint p) {}
	virtual void mouseMoveEvent() {}
	virtual void mouseReleaseEvent() {}
	virtual void paintEvent(QPainter* painter) {}

private:
	string mName;
	QWidget* mViewport;
	Scene* mScene;
	Camera* mCamera;
};

class DrawLineState : public State
{
public:
	DrawLineState(string name, metadata* data);

	virtual void mousePressEvent(QPoint p);
	virtual void mouseMoveEvent() {}
	virtual void mouseReleaseEvent() {}
	virtual void paintEvent(QPainter* painter);

private:
	string mName;
	Viewport* mViewport;
	Scene* mScene;
	Camera* mCamera;
};
