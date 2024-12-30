#pragma once
#include "includes.h"
#include "Scene.h"
#include "ECS.h"

class OpeningScene : public Scene {
public:
	Entity* NPCEntity;

	void init() override;

	void update() override;

	void end() override;
};

class FirstHouseScene : public Scene {
public:

	void init() override;

	void update() override;

	void end() override;
};