#pragma once
#include "GameMap.h"
#include "ECS.H"
#include "Components.h"
#include "GameStateManager.h"
#include "TheaterEngine.h"
#include "includes.h"

class GameMap;

class GameEngine
{
public:
	GameEngine(
		uint32_t nWidth, 
		uint32_t nHeight, 
		const std::string& title, 
		uint32_t fps
		);

	~GameEngine();

	void handleEvents();

	void handeKeyInputBools();

	void update();

	void draw();

	void sortEntityArray();

	bool alive();

	SDL_Renderer* getRen() const;

	const uint32_t getWinSize() const;

	void renderText();

	void test_NPCMoveFunction();

	const void test_portalAnimationFunction();

	// return true if successful change
	const bool changeCurrentMap(size_t newId);

private:

	uint32_t nWinWidth, nWinHeight;
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Event* Event;
	//uint32_t unStartElapsedTime;
	//int nElapsedTime;
	int currentKey;
	uint32_t numberOfFramesSinceStart;
	uint32_t nFps;
	uint32_t nDelay;
	int mousePos[2];
	std::string sTitle;
	std::vector<Entity*>* arrayOfActiveEntities;
	std::vector<Entity*> bufferArrayOfEntities;

	// key down bools
	bool w_keyDown;
	bool a_keyDown;
	bool s_keyDown;
	bool d_keyDown;

	// game entities
	Manager m_entityManager;
	Entity* playerEntity;
	Entity* NPCEntity;
	
	// state manager
	GameStateManager* m_stateManager;

	// text
	TTF_Font* arialFont;
	std::string textMessage;
	std::string lastMessage;
	SDL_Surface* surfaceMessage;
	SDL_Texture* Message;

	// music
	Mix_Music* gMusic;

	// gamemap instance
	GameMap* Town1;
	GameMap* House1;

	size_t currentMapId;
	std::array<GameMap*, 2> mapArray_firstSection;

	// theater engine instance
	TheaterEngine* theaterEngine;

	bool bRunning;
protected:
};


