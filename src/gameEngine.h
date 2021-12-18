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

	bool alive();

	SDL_Renderer* getRen() const;

	int getWinSize() const;

private:

	int nWinWidth, nWinHeight;
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Event* Event;
	//uint32_t unStartElapsedTime;
	//int nElapsedTime;
	int currentKey;
	int nFps;
	int nDelay;
	int mousePos[2];
	std::string sTitle;

	// game entities
	Manager m_entityManager;
	Entity* playerEntity;
	
	// state manager
	GameStateManager* m_stateManager;

	GameMap* map;

	bool bRunning;
protected:
};


