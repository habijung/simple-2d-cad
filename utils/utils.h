#pragma once


// Circular Dependency
class Viewport;
class Scene;
class Camera;

struct metadata
{
	Viewport* viewport;
	Scene* scene;
	Camera* camera;
};
