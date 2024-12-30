#include "Scene.h"
#include "includes.h"
#include "ECS.h"

class OpeningScene : Scene {
public:
	OpeningScene();

	void init() override;

	void update() override;

	void end() override;
};

class FirstHouseScene : Scene {
public:
	void init() override;

	void update() override;

	void end() override;
};