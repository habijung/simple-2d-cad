#pragma once

#include <string>
#include <utility>
#include <QWidget>
#include <QMouseEvent>
#include "../viewport/Scene.h"
#include "../viewport/Camera.h"
#include "../shape/Shape.h"
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
	virtual void updateScene(Scene* scene) = 0;
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

	virtual void updateScene(Scene* scene) override;
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
	vector<QPointF> mPoints; // Line 구성을 위한 임시 points vector
	int mButton;
	bool mDrawLine;
};

class DrawFaceState : public State
{
public:
	DrawFaceState(string name, component* comp);

	virtual void updateScene(Scene* scene) override;
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

	QPolygonF mPolygon;
	QPolygonF mGuidePolygon;
	QLineF mLine;
	Vertex* mVertex;
	vector<QPointF> mPoints; // Polygon 구성을 위한 임시 points vector
	bool mDrawPolygon;
	bool mHit;
};

class SelectPointState : public State
{
public:
	SelectPointState(string name, component* comp);

	virtual void updateScene(Scene* scene) override;
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

	// For snapping
	QPolygonF mSnapPolygon;
	QPointF mSnapPoint;
	Vertex mSnapVertex;
	bool mSnap;
};

class SelectLineState : public State
{
public:
	SelectLineState(string name, component* comp);

	virtual void updateScene(Scene* scene) override;
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
	QPointF mPosStart;
	vector<Vertex> mLineVertices;
	int mButton;

	// For select line
	list<Shape*> mShapes;
	Line* mLine;
	QPolygonF mPolygon;
	bool mHit;
};

class SelectFaceState : public State
{
public:
	SelectFaceState(string name, component* comp);

	virtual void updateScene(Scene* scene) override;
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
	QPointF mPosStart;
	vector<Vertex> mFaceVertices;
	int mButton;

	// For select line
	list<Shape*> mShapes;
	Face* mFace;
	QPolygonF mPolygon;
	bool mHit;
};
