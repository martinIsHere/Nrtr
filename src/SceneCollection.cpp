#include "SceneCollection.h"
#include "components.h"
#include "ECS.h"
#include "TheaterEngine.h"


bool isViableInteractCoords(Point* interactionCoords) {
	if (interactionCoords->x < 0) return false;
	return true;
}

OpeningScene::OpeningScene() {
	NPCEntity = nullptr;
	Town1 = nullptr;
	frames = NULL;
}

void OpeningScene::init()  {
	frames = 0;

	// creating player
	if (!Scene::playerEntity) {
		Scene::playerEntity = Scene::entityManagerPtr->addEntity();
		Scene::playerEntity->addComponent<PositionComponent>(TILE_SIZE_PIXELS, TILE_SIZE_PIXELS);
	}

	// loading maps
	if (!Town1) {
		Town1 = new GameMap(
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
	}
	*currentMapPtrPtr = Town1;

	if (!Scene::playerEntity->hasComponent<DrawingComponent>()) {
		playerEntity->addComponent<DrawingComponent>(
			renPtr,    // current working renderer
			"res/imgs/hero.bmp", // path
			16, // sprite size
			6, 5, // sprite sheet columns and rows
			4, // amount of frames/imgs of walking animation
			8,  // amount of animation frames per second
			Town1->getCam() // current working camera
			);
		playerEntity->addComponent<CollisionComponent>(Town1); // pass in map class
		playerEntity->addComponent<InteractionComponent>(Town1, interactionCoords, playerHasInteracted);
	}

	if (!NPCEntity) {
		NPCEntity = entityManagerPtr->addEntity();

		NPCEntity->addComponent<PositionComponent>(TILE_SIZE_PIXELS, TILE_SIZE_PIXELS);

		NPCEntity->addComponent<DrawingComponent>(
			renPtr,
			"res/imgs/secondNPC.bmp",
			16,
			3, 4,
			2,// amount of animation frames per second
			4, // amount of animation frames per second
			Town1->getCam()
			);

		NPCEntity->addComponent<CollisionComponent>(Town1);

		NPCEntity->getComponent<PositionComponent>().set_isFrictionless(true);

		NPCEntity->getComponent<PositionComponent>().set_default_acceleration(0);
	}
}

void OpeningScene::update()  {
	if (*playerHasInteracted && isViableInteractCoords(interactionCoords)) {
		*playerHasInteracted = false;
		if (interactionCoords->isEqual(new Point(7,5))
			|| interactionCoords->isEqual(new Point(8, 5))) {
			if (theaterEnginePtr != nullptr) { 
			//(*theaterEnginePtr).changeCurrentScene<FirstHouseScene>();
			}
		}
	}

	if (frames < 228) {
		NPCEntity->getComponent<PositionComponent>().setVel(4, 0);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, true);
		NPCEntity->getComponent<PositionComponent>().setFacingDir(DIR_RIGHT);
	}
	else if (frames < 800) {
		NPCEntity->getComponent<PositionComponent>().setVel(-2.8f, 2.8f);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, false);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, true);
		NPCEntity->getComponent<PositionComponent>().setFacingDir(DIR_LEFT);
	}
	else if (frames < 1400) {
		NPCEntity->getComponent<PositionComponent>().setVel(2.8f, 2.8f);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, false);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, true);
		NPCEntity->getComponent<PositionComponent>().setFacingDir(DIR_RIGHT);
	}
	else if (frames < 1600) {
		NPCEntity->getComponent<PositionComponent>().setVel(-2.8f, 0);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, false);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, true);
		NPCEntity->getComponent<PositionComponent>().setFacingDir(DIR_LEFT);
	}
	else if (frames < 1700) {
		NPCEntity->getComponent<PositionComponent>().setVel(0, 2.8f);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, false);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_DOWN, true);
		NPCEntity->getComponent<PositionComponent>().setFacingDir(DIR_DOWN);
	}
	else {
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_DOWN, false);
		NPCEntity->getComponent<PositionComponent>().setVel(0, 0);
	}
	frames++;
}

void OpeningScene::draw() {
	SDL_Rect box = { 
		interactionCoords->x * TILE_SIZE_PIXELS - *(*currentMapPtrPtr)->getCam()->getOffsetXPtr(),
		interactionCoords->y * TILE_SIZE_PIXELS - *(*currentMapPtrPtr)->getCam()->getOffsetYPtr(),
		TILE_SIZE_PIXELS, 
		TILE_SIZE_PIXELS };
	SDL_SetRenderDrawColor(renPtr, 255, 0, 0, 255);
	SDL_RenderDrawRect(renPtr, &box);
}

void OpeningScene::end()  {

}

/*#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#*/

FirstHouseScene::FirstHouseScene() {
	House1 = nullptr;
	frames = NULL;
}

void FirstHouseScene::init() {
	if (!House1) {
		House1 = new GameMap(
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
	}
}

void FirstHouseScene::update() {

}

void FirstHouseScene::draw() {

}

void FirstHouseScene::end() {

}

/*#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#¤#*/

