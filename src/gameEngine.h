#pragma once
#include "GameMap.h"
#include "ECS.H"
#include "Components.h"
#include "GameStateManager.h"
#include "includes.h"

class GameMap;

class GameEngine
{
public:
	GameEngine(uint32_t nWidth, uint32_t nHeight, const std::string& title, uint32_t fps);

	~GameEngine();

	void handleEvents();

	void update();

	void draw();

	void sortEntityArray();

	bool alive();

	SDL_Renderer* getRen() const;

	const uint32_t getWinSize() const;

	void test_NPCMoveFunction();

	void renderText();

	const void test_portalAnimationFunction();

private:

	uint32_t nWinWidth, nWinHeight;
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Event* Event;
	//uint32_t unStartElapsedTime;
	//int nElapsedTime;
	int currentKey;
	int numberOfFramesUntilNow;
	uint32_t nFps;
	uint32_t nDelay;
	int mousePos[2];
	std::string sTitle;
	std::vector<Entity*>* arrayOfActiveEntities;
	std::vector<Entity*> bufferArrayOfEntities;

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

	GameMap* map;

	bool bRunning;
protected:
};


