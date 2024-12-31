#pragma once
#include "includes.h"
#include "Scene.h"
#include "ECS.h"

struct gameInteractionEvent {
	int x, y; // coordinate of block that triggers event
	int playerNewX, playerNewY; // new player coordinates to be teleported to after switch
	int ID; // ID of scene be activated
};

#define MAX_SIZE_DOOR_INTERACTIONS 2

class OpeningScene : public Scene {
public:
	const std::array<Point, MAX_SIZE_DOOR_INTERACTIONS> doorInteractionArray;
	Entity* NPCEntity;
	uint32_t frames;
	GameMap* Town1;

	const int ID = 0;

	OpeningScene();

	void init() override;

	void update() override;

	void draw() override;

	void end() override;
};

class FirstHouseScene : public Scene {
public:
	uint32_t frames;
	GameMap* House1;

	const int ID = 1;

	FirstHouseScene();

	void init() override;

	void update() override;

	void draw() override;

	void end() override;
};