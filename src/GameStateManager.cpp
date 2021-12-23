#include "GameStateManager.h"

GameStateManager::GameStateManager() {
	state = new GameState;
	*state = state_noState;
}

GameStateManager::~GameStateManager() {
	delete state;
}

GameStateManager::GameState& GameStateManager::get() {
	return *state;
}