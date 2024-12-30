#pragma once
#include "includes.h"
#include "GameStateManager.h"

class TheaterEngine {
	 GameStateManager* gameStateManagerPtr;
	 Scene* currentScenePtr;
	 Manager* entityManagerPtr;
	 std::vector<Scene*>* currentlyLoadedScenesPtr;
public:
	TheaterEngine();

	~TheaterEngine();

	void init(
		GameStateManager* gameStateManagerPtr,
		Manager* entityManagerPtr,
		std::array<GameMap*, 10>* mapArrayPtr,
		size_t* currentMapIdPtr,
		std::vector<Entity*>* arrayOfActiveEntitiesPtr,
		SDL_Renderer* renPtr,
		uint32_t* nWinWidthPtr,
		uint32_t* nWinHeightPtr
		);

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

	Entity* getPlayerEntity();

	void update();
private:

};