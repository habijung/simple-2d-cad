#pragma once
#include <QWidget>
#include <vector>
#include <utility>
#include "Scene.h"
#include "Camera.h"
#include "../state/StateMachine.h"

class Viewport : public QWidget
{
public:
	Viewport(QWidget* parent = nullptr);
	~Viewport();

	// Update Scene
	void UpdateState(const std::string& name);
	void SaveScene();
	void LoadScene();
	void CreateNewScene();

private:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

	int mButton;
	QPointF mPos;

	// Viewport data
	SelectUtils::ViewportData* mData;
	Scene* mScene;
	Camera* mCamera;
	StateMachine* mMachine;
};
