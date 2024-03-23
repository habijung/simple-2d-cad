#pragma once
#include <utility>
#include <QWidget>
#include <QMouseEvent>
#include "../viewport/Scene.h"
#include "../viewport/Camera.h"
#include "../shape/Shape.h"
#include "../utils/SelectUtils.h"

class Viewport;

class State
{
public:
	State(const std::string& name, SelectUtils::ViewportData* data);
	std::string GetName();
	bool GetMouseLeftPressed(bool hit, int button, QMouseEvent* event);

	virtual void UpdateScene(Scene* scene) = 0;
	virtual void mousePressEvent(QMouseEvent* event) = 0;
	virtual void mouseMoveEvent(QMouseEvent* event) = 0;
	virtual void mouseReleaseEvent(QMouseEvent* event) = 0;
	virtual void paintEvent(QPainter* painter) = 0;

private:
	std::string mName;
	QWidget* mViewport;
	Scene* mScene;
	Camera* mCamera;
};

class DrawLineState : public State
{
public:
	DrawLineState(const std::string& name, SelectUtils::ViewportData* data);

	virtual void UpdateScene(Scene* scene) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPainter* painter) override;

private:
	std::string mName;
	Viewport* mViewport;
	Scene* mScene;
	Camera* mCamera;

	QPointF mPos;
	std::vector<QPointF> mPoints; // Temporary points for line construction
	int mButton;
	bool mDrawLine;
};

class DrawFaceState : public State
{
public:
	DrawFaceState(const std::string& name, SelectUtils::ViewportData* data);

	virtual void UpdateScene(Scene* scene) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPainter* painter) override;

private:
	std::string mName;
	Viewport* mViewport;
	Scene* mScene;
	Camera* mCamera;

	QPointF mPos;
	int mButton;

	QPolygonF mPolygon;
	QPolygonF mGuidePolygon;
	QLineF mLine;
	Vertex* mVertex;
	std::vector<QPointF> mPoints; // Temporary points for polygon construction
	bool mDrawPolygon;
	bool mHit;
};

class SelectPointState : public State
{
public:
	SelectPointState(const std::string& name, SelectUtils::ViewportData* data);

	virtual void UpdateScene(Scene* scene) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPainter* painter) override;

private:
	std::string mName;
	Viewport* mViewport;
	Scene* mScene;
	Camera* mCamera;

	QPointF mPos;
	int mButton;

	// For select point
	std::list<Shape*> mShapes;
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
	SelectLineState(const std::string& name, SelectUtils::ViewportData* data);

	virtual void UpdateScene(Scene* scene) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPainter* painter) override;

private:
	std::string mName;
	Viewport* mViewport;
	Scene* mScene;
	Camera* mCamera;

	QPointF mPos;
	QPointF mPosStart;
	std::vector<Vertex> mLineVertices;
	int mButton;

	// For select line
	std::list<Shape*> mShapes;
	Line* mLine;
	QPolygonF mPolygon;
	bool mHit;
};

class SelectFaceState : public State
{
public:
	SelectFaceState(const std::string& name, SelectUtils::ViewportData* data);

	virtual void UpdateScene(Scene* scene) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPainter* painter) override;

private:
	std::string mName;
	Viewport* mViewport;
	Scene* mScene;
	Camera* mCamera;

	QPointF mPos;
	QPointF mPosStart;
	std::vector<Vertex> mFaceVertices;
	int mButton;

	// For select line
	std::list<Shape*> mShapes;
	Face* mFace;
	QPolygonF mPolygon;
	bool mHit;
};
