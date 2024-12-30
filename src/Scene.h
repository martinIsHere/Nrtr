#pragma once
#include "includes.h"

class Entity;
class Manager;

class Scene {
public:
	static Manager* entityManagerPtr;
	static std::array<GameMap*, 10>* mapArrayPtr;
	static size_t* currentMapIdPtr;
	static std::vector<Entity*>* arrayOfActiveEntitiesPtr;
	static SDL_Renderer* renPtr;
	static uint32_t* nWinWidthPtr, * nWinHeightPtr;
	static Entity* playerEntity;

	Scene() {}

	virtual void init() {

	}

	virtual void update() {

	}

	virtual void end() {

	}

private:

};
