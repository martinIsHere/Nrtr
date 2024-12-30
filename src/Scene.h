#pragma once
#include "includes.h"

class Entity;
class Manager;

class Scene {
	std::vector<Entity*>* entityArrayPtr;
	Manager* entityManager;
public:
	Scene() {
		entityArrayPtr = nullptr;
		entityManager = nullptr;
	}

	virtual void init() {

	}

	virtual void update() {

	}

	virtual void end() {

	}

private:

};