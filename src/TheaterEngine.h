#pragma once
#include "includes.h"
#include "ECS.h"
#include "GameStateManager.h"

class TheaterEngine {
	 GameStateManager::GameState* gameState;
public:
	TheaterEngine();

	~TheaterEngine();

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

	void update();
private:

};