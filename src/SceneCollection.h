#pragma once
#include "includes.h"
#include "Scene.h"

class OpeningScene : public Scene {
public:
	OpeningScene();

	void init() override;

	void update() override;

	void end() override;
};

class FirstHouseScene : public Scene {
public:
	FirstHouseScene();

	void init() override;

	void update() override;

	void end() override;
};