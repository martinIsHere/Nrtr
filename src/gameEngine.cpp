#include "GameEngine.h"

GameEngine::GameEngine(const int nWidth, const int nHeight, const std::string& title, const int fps) {
	nElapsedTime = 0;
	unStartElapsedTime = 0;
	nWinWidth = nWidth;
	nWinHeight = nHeight;
	bRunning = true;
	nFps = fps;
	sTitle = title;
	currentKey = 0;
	nDelay = 1000 / nFps;
	mousePos[0] = 0, mousePos[1] = 0;
	


	// game state related stuff
	m_stateManager = new GameStateManager();

	m_stateManager->get() = m_stateManager->startingScreen;

	// when game has fully begun
	m_stateManager->get() = m_stateManager->game;



	
	//setup SDL stuff
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed to create window\n";
		bRunning = false;
		exit(1);
	}

	Event = new SDL_Event();
	win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nWinWidth, nWinHeight, 0);
	if (win == 0) {
		std::cout << "Failed to create window\n";
		bRunning = false;
		exit(1);
	}
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == 0) {
		std::cout << "Failed to create renderer\n";
		bRunning = false;
		exit(1);

	}
	

	//player1 = new Player(ren, map, 64, "res/imgs/hero.bmp", 16, 3, 4);

	playerEntity = m_entityManager.addEntity();
	playerEntity->addComponent<PositionComponent>(0, 0);

	map = new GameMap(
		ren,
		"res/map/map.bin",
		nWinWidth, nWinHeight,
		&(playerEntity->getComponent<PositionComponent>().getx()),
		&(playerEntity->getComponent<PositionComponent>().gety())
		);

	playerEntity->addComponent<DrawingComponent>(
		ren, 
		"hero.bmp", 
		3, 4, 
		nFps, 
		map->getCam()->getOffsetX(), map->getCam()->getOffsetY());


	m_entityManager.init();

}

GameEngine::~GameEngine() {
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
}

void GameEngine::handleEvents() {
	while (SDL_PollEvent(Event)) {
		switch (Event->type) {
		case SDL_QUIT:
			bRunning = false;
			break;

		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mousePos[0], &mousePos[1]);
			break;

		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_KEYDOWN:
			switch (Event->key.keysym.sym) {
			case SDLK_w:
				// set to up
				playerEntity->getComponent<PositionComponent>().setDirY_down(0);
				playerEntity->getComponent<PositionComponent>().setDirY_up(1);
				break;
			case SDLK_s:
				// set to down
				playerEntity->getComponent<PositionComponent>().setDirY_down(1);
				break;
			case SDLK_a:
				playerEntity->getComponent<PositionComponent>().setDirX_left(1);
				break;
			case SDLK_d:
				playerEntity->getComponent<PositionComponent>().setDirX_left(0);
				playerEntity->getComponent<PositionComponent>().setDirX_right(1);
				break;
			}
			break;
		case SDL_KEYUP:
			switch (Event->key.keysym.sym) {
			case SDLK_w:
				playerEntity->getComponent<PositionComponent>().setDirY_up(0);
				break;
			case SDLK_s:
				playerEntity->getComponent<PositionComponent>().setDirY_down(0);
				break;
			case SDLK_a:
				playerEntity->getComponent<PositionComponent>().setDirX_left(0);
				break;
			case SDLK_d:
				playerEntity->getComponent<PositionComponent>().setDirX_right(0);
				break;
			}
			break;
		}
	}
}

void GameEngine::update() {
	unStartElapsedTime = SDL_GetTicks();

	if (m_stateManager->get() == m_stateManager->game) {

		handleEvents();
		//player1->move();

		// update all entities
		m_entityManager.update();
		playerEntity->getComponent<PositionComponent>().setAcc(0, 0);

		// 
		draw();

		map->update();
	
		// delay
		nElapsedTime = SDL_GetTicks() - unStartElapsedTime;
		if (nDelay > nElapsedTime) {
			SDL_Delay(nDelay - nElapsedTime);
		}
		if ((SDL_GetTicks() - unStartElapsedTime) != 0) {
			actualFPS = 1000 / (SDL_GetTicks() - unStartElapsedTime);
		}
		std::string bufTitle = "Tilebased Game, FPS: " + std::to_string(actualFPS);
		SDL_SetWindowTitle(win, bufTitle.c_str());
	}
}

void GameEngine::draw() {

	// clear screen
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderClear(ren);

	if (m_stateManager->get() == m_stateManager->game) {

		map->draw();

		m_entityManager.draw();
	}

	SDL_RenderPresent(ren);
}



bool GameEngine::alive() {
	return bRunning;
}

SDL_Renderer* GameEngine::getRen() const {
	return ren;
}

int GameEngine::getWinSize() const {
	return nWinWidth, nWinHeight;
}