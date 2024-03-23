#pragma once


// Circular Dependency
class Viewport;
class Scene;
class Camera;

struct component
{
	Viewport* viewport;
	Scene* scene;
	Camera* camera;
};

struct Point
{
	int x;
	int y;
};
