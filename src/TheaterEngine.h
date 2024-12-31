#pragma once
#include "includes.h"
#include "GameStateManager.h"

#define MAX_LOADED_SCENES 10

class TheaterEngine {
	 GameStateManager* gameStateManagerPtr;
	 Scene* currentScenePtr;
	 Manager* entityManagerPtr;
	 std::array<Scene*, MAX_LOADED_SCENES>* currentlyLoadedScenesPtr;
public:
	TheaterEngine();

	~TheaterEngine();

	void init(
		GameStateManager* gameStateManagerPtr,
		Manager* entityManagerPtr,
		GameMap** currentMapPtrPtr,
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

	template <typename T>
	Scene* createOrGetScene()const;

	bool changeCurrentMap(GameMap* in_currentMapPtr);

	template <typename T>
	bool changeCurrentScene();

	Entity* getPlayerEntity();

	GameMap* getCurrentMap();

	void update();

	void draw();

private:

};