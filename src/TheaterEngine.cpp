#include "TheaterEngine.h"
#include "PositionComponent.h"

TheaterEngine::TheaterEngine() {
	gameState = nullptr;
}

TheaterEngine::~TheaterEngine() {
	delete gameState;
}

void TheaterEngine::init(GameStateManager::GameState* gameState) {
	this->gameState = gameState;
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