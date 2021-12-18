#pragma once

#include "includes.h"

class GameStateManager{

public:
	// all possible gamestates
	enum GameState : uint32_t {
		noState=0,
		loading,
		startingScreen,
		startToGame_fade,
		game,
		win,
		death,
		pauseMenu,
		mainMenu,
		optionMenu,
		aboutMenu
	};
	
private:
	//current gamestate
	GameState* state;

public:

	GameStateManager();

	~GameStateManager();

	// getters
	GameState& get();
};

