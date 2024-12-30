#include "SceneCollection.h"
#include "components.h"
#include "ECS.h"

void OpeningScene::init()  {
	if (Scene::entityManagerPtr == nullptr) log("entityManagerPtr == nullptr");
	Scene::playerEntity = Scene::entityManagerPtr->addEntity();
	//NPCEntity = entityManagerPtr->addEntity();
	if (Scene::playerEntity->hasComponent<PositionComponent>()) log("playerEntity->hasComponent<PositionComponent>");
	Scene::playerEntity->addComponent<PositionComponent>(TILE_SIZE_PIXELS, TILE_SIZE_PIXELS);
	//NPCEntity->addComponent<PositionComponent>(TILE_SIZE_PIXELS, TILE_SIZE_PIXELS);
	if (Scene::playerEntity->hasComponent<PositionComponent>())
		log(Scene::playerEntity->getComponent<PositionComponent>().getx());
	log("asdf");
	GameMap* Town1 = new GameMap(
		Scene::renPtr,
		"res/map/Town1.bin", // path to map file
		"res/imgs/sh1.bmp",  // path to spritesheet file
		12, // amount of tiles horizontally in spritesheet
		18, // amount of tiles vertically in spritesheet
		*Scene::nWinWidthPtr, *Scene::nWinHeightPtr, // window-to-be-displayed-on's width and heigth
		&(Scene::playerEntity->getComponent<PositionComponent>().getx()), // pointer to coordinates for camera
		&(Scene::playerEntity->getComponent<PositionComponent>().gety()), // here: just equal to the player position
		false // if the map is very small, like a house -> set to true
		);

	GameMap* House1 = new GameMap(
		renPtr,
		"res/map/House1.bin",
		"res/imgs/sh2.bmp",
		7,
		12,
		*nWinWidthPtr, *nWinHeightPtr,
		&(playerEntity->getComponent<PositionComponent>().getx()),
		&(playerEntity->getComponent<PositionComponent>().gety()),
		true
		);

	*currentMapIdPtr = 0; // for Town1
	*mapArrayPtr = { Town1, House1 };

	if (Town1->getCam() == nullptr) log("Town1->getCam() == nullptr  idk why ;-;");

	if (playerEntity->hasComponent<DrawingComponent>()) log("playerEntity->hasComponent<DrawingComponent>");
	playerEntity->addComponent<DrawingComponent>(
		renPtr,    // current working renderer
		"res/imgs/hero.bmp", // path
		16, // sprite size
		6, 5, // sprite sheet columns and rows
		4, // amount of frames/imgs of walking animation
		8,  // amount of animation frames per second
		Town1->getCam() // current working camera
		);
	/*
	NPCEntity->addComponent<DrawingComponent>(
		renPtr,
		"res/imgs/secondNPC.bmp",
		16,
		3, 4,
		2,// amount of animation frames per second
		4, // amount of animation frames per second
		Town1->getCam()
		);
	*/

	playerEntity->addComponent<CollisionComponent>(Town1); // pass in map class
	//NPCEntity->addComponent<CollisionComponent>(Town1);
	//NPCEntity->getComponent<PositionComponent>().set_isFrictionless(true);
	//NPCEntity->getComponent<PositionComponent>().set_default_acceleration(0);

	playerEntity->addComponent<InteractionComponent>(Town1);


	//entityManagerPtr->init();

}

void OpeningScene::update()  {

}

void OpeningScene::end()  {

}

/*#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#*/

void FirstHouseScene::init() {

}

void FirstHouseScene::update() {

}

void FirstHouseScene::end() {

}

/*#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#*/

