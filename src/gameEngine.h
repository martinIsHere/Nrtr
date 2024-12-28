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
	GameEngine(int nWidth, int nHeight, const std::string& title, int fps);

	~GameEngine();

	void handleEvents();

	void update();

	void draw();

	void sortEntityArray();

	bool alive();

	SDL_Renderer* getRen() const;

	int getWinSize() const;

	void test_NPCMoveFunction();

	void renderText();

private:

	int nWinWidth, nWinHeight;
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Event* Event;
	//uint32_t unStartElapsedTime;
	//int nElapsedTime;
	int currentKey;
	int numberOfFramesUntilNow;
	int nFps;
	int nDelay;
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

	GameMap* map;

	bool bRunning;
protected:
};


