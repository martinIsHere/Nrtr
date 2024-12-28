#pragma once
#include "includes.h"
#include "ECS.h"
#include "GameStateManager.h"

class TheaterEngine {
	uint32_t numberOfFramesSinceStart;
	GameStateManager::GameState* gameState;
public:
	TheaterEngine();

	void init(GameStateManager::GameState* gameState);

	void makeEntityMove_constantSpeedNoDiagonalMovement(
		Entity* ent, 
		int xDest, 
		int yDest, 
		float speed, 
		int waitTimeFirstStop,
		int waitTimeSecondStop,
		bool xMove_before_yMove
		);
	void teleportEntity(
		Entity* ent, 
		int xDest, 
		int yDest
		);

	void update(
		const uint32_t numberOfFramesSinceStart
		);
private:

};