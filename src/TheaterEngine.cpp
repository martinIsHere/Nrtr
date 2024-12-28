#include "TheaterEngine.h"

TheaterEngine::TheaterEngine() {

}

void TheaterEngine::init(GameStateManager::GameState* gameState) {
	this->gameState = gameState;
}

void TheaterEngine::makeEntityMove_constantSpeedNoDiagonalMovement(
	Entity* ent,
	int xDest,
	int yDest,
	float speed,
	int waitTimeFirstStop,
	int waitTimeSecondStop,
	bool xMove_before_yMove
	) {

}
void TheaterEngine::teleportEntity(
	Entity* ent,
	int xDest,
	int yDest
	) {

}

void TheaterEngine::update(
	const uint32_t numberOfFramesSinceStart
	) {

}