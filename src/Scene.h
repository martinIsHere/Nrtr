#pragma once
#include "includes.h"
#include "ECS.h"

class Scene {
	std::vector<Entity*>* entityArrayPtr;
	Manager* entityManager;
public:

	virtual void init();

	virtual void update();

	virtual void end();

private:

};