#include "TheaterEngine.h"
#include "SceneCollection.h"
#include "Components.h"
//#include "Scene.h"

TheaterEngine::TheaterEngine() {
	gameState = nullptr;
	currentScene = nullptr;
	entityManager = nullptr;
}

TheaterEngine::~TheaterEngine() {
	delete gameState;
}

void TheaterEngine::init(
	GameStateManager::GameState* gameState,
	Manager* entityManager
	) {
	this->entityManager = entityManager;
	this->gameState = gameState;
	// TODO: relative to prev save init a scene
	currentScene = new OpeningScene();
	currentScene->init();
}

void TheaterEngine::makeEntityMove_constantSpeedNoDiagonalMovement(
	Entity* ent,
	int xDest,
	int yDest,
	float speed,
	const int waitTimeFirstStop_inSeconds, 
	const int waitTimeSecondStop_inSeconds, 
	bool xMove_before_yMove
	) {
	int waitTimeFirstStop_inFrames = waitTimeFirstStop_inSeconds * targetFPS;
	int waitTimeSecondStop_inFrames = waitTimeSecondStop_inSeconds * targetFPS;

}
void TheaterEngine::teleportEntity(
	Entity* ent,
	int xDest,
	int yDest
	) {
	ent->getComponent<PositionComponent>().setPos(xDest, yDest);
}

void TheaterEngine::update() {

}