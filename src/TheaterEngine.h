#pragma once
#include "includes.h"
#include "ECS.h"

class TheaterEngine {

public:
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
private:

};