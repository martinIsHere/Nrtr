#pragma once
#include "includes.h"

class Entity;
class Manager;
struct gameInteractionEvent;

class Scene {
public:
	static Manager* entityManagerPtr;
	static GameMap** currentMapPtr;
	static std::vector<Entity*>* arrayOfActiveEntitiesPtr;
	static SDL_Renderer* renPtr;
	static uint32_t* nWinWidthPtr, * nWinHeightPtr;
	static Entity* playerEntity;
	static Point* interactionCoords;
	static bool* playerHasInteracted;

	Scene() {}

	virtual void init() {

	}

	virtual void update() {

	}

	virtual void draw() {

	}

	virtual void end() {

	}

private:

};
