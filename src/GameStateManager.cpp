#include "GameStateManager.h"

GameStateManager::GameStateManager() {
	state = new GameState;
	*state = noState;
}

GameStateManager::~GameStateManager() {
	delete state;
}

GameStateManager::GameState& GameStateManager::get() {
	return *state;
}