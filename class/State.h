#pragma once

#include <string>
#include <utility>
#include <QWidget>
#include <QMouseEvent>
#include "Scene.h"
#include "Camera.h"
#include "Shape.h"
#include "../utils/utils.h"
using namespace std;


class Viewport; // Circular Dependency

class State
{
public:
	State(string name, component* comp);
	string getStateName();
	bool getMouseLeftPressed(bool hit, int button, QMouseEvent* event);

	virtual ~State() {}
	virtual void mousePressEvent(QMouseEvent* event) = 0;
	virtual void mouseMoveEvent(QMouseEvent* event) = 0;
	virtual void mouseReleaseEvent(QMouseEvent* event) = 0;
	virtual void paintEvent(QPainter* painter) = 0;

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

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPainter* painter) override;

private:
	string mName;
	Viewport* mViewport;
	Scene* mScene;
	Camera* mCamera;

	vector<QPointF> mPoints; // Line 구성을 위한 임시 points vector
	QPointF mPos;
	bool mDrawLine;
	int mButton;
};

class DrawFaceState : public State
{
public:
	DrawFaceState(string name, component* comp);

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override {}
	virtual void mouseReleaseEvent(QMouseEvent* event) override {}
	virtual void paintEvent(QPainter* painter) override;

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

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPainter* painter) override;

private:
	string mName;
	Viewport* mViewport;
	Scene* mScene;
	Camera* mCamera;

	QPointF mPos;
	int mButton;

	// For select point
	list<Shape*> mShapes;
	Vertex* mVertex;
	QPolygonF mPolygon;
	bool mHit;
};

class SelectLineState : public State
{
public:
	SelectLineState(string name, component* comp);

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPainter* painter) override;

private:
	string mName;
	Viewport* mViewport;
	Scene* mScene;
	Camera* mCamera;

	QPointF mPos;
	int mButton;

	// For select line
	list<Shape*> mShapes;
	Line* mLine;
	QPolygonF mPolygon;
	bool mHit;
};
