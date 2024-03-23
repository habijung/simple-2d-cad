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
	State(string name, component* comp);
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
	DrawLineState(string name, component* comp);

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

class DrawFaceState : public State
{
public:
	DrawFaceState(string name, component* comp);

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

class SelectPointState : public State
{
public:
	SelectPointState(string name, component* comp);

private:
	string mName;
	Viewport* mViewport;
	Scene* mScene;
	Camera* mCamera;
};
