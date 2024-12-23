#include "GameStateManager.h"

GameStateManager::GameStateManager() {
	state = new GameState;
	*state = state_noState;
}

GameStateManager::~GameStateManager() {
	delete state;
}

void GameStateManager::set(GameState state) {
	*this->state = state;
}

GameStateManager::GameState GameStateManager::get() {
	return *state;
}

