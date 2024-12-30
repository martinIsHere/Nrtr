#pragma once
#include "includes.h"
#include "Scene.h"
#include "ECS.h"

struct gameInteractionEvent {
	int x, y; // coordinate of block that triggers event
	int playerNewX, playerNewY; // new player coordinates to be teleported to after switch
	int ID; // ID of scene be activated
};

class OpeningScene : public Scene {
public:
	Entity* NPCEntity;
	uint32_t frames;
	GameMap* Town1;

	constexpr static int ID = 0;

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

	constexpr static int ID = 1;

	FirstHouseScene();

	void init() override;

	void update() override;

	void draw() override;

	void end() override;
};