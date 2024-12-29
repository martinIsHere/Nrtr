#include "GameEngine.h"

GameEngine::GameEngine(const uint32_t nWidth, const uint32_t nHeight, const std::string& title, const uint32_t fps) {
	nElapsedTime = 0;
	unStartElapsedTime = 0;
	nWinWidth = nWidth;
	nWinHeight = nHeight;
	bRunning = true;
	nFps = fps;
	targetFPS = fps; // quickfix
	sTitle = title;
	currentKey = 0;
	nDelay = 1000 / nFps;
	mousePos[0] = 0, mousePos[1] = 0;
	numberOfFramesSinceStart = 0;


	// game state related stuff
	m_stateManager = new GameStateManager();

	m_stateManager->set(m_stateManager->state_startingScreen);

	// when game has fully started
	m_stateManager->set(m_stateManager->state_gameRunning);



	//setup SDL stuff
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed to create window\n";
		bRunning = false;
		exit(1);
	}

	if (TTF_Init() < 0) {
		std::cout << "Failed to create window\n";
		bRunning = false;
		exit(1);
	}

	// load font
	arialFont = TTF_OpenFont("res/fonts/arial.ttf", 80);
	if (arialFont == nullptr) { log(TTF_GetError()); log("failed to load font")}
	textMessage = "";
	lastMessage = "";
	surfaceMessage = nullptr;
	Message = nullptr;
	
	//win stuff
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
	

	//music ------------------------

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		bRunning = false;
	}

	gMusic = Mix_LoadMUS("res/audio/Sorry im late, traffic.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		bRunning = false;
	}

	Mix_VolumeMusic(MIX_MAX_VOLUME * 0.5);

	if (Mix_PlayingMusic() == 0)
	{
		//Play the music
		Mix_PlayMusic(gMusic, -1);
	}

	theaterEngine = new TheaterEngine();
	theaterEngine->init(m_stateManager->getPtr());

	playerEntity = m_entityManager.addEntity();
	NPCEntity = m_entityManager.addEntity();
	playerEntity->addComponent<PositionComponent>(60, 60);
	NPCEntity->addComponent<PositionComponent>(60, 60);

	map = new GameMap(
		ren,
		"res/map/Town1.bin",
		nWinWidth, nWinHeight,
		&(playerEntity->getComponent<PositionComponent>().getx()),
		&(playerEntity->getComponent<PositionComponent>().gety())
		);

	// get reference actually
	map->get_drawingSolidStates_bool() = false;
	playerEntity->addComponent<DrawingComponent>(
		ren, 
		"res/imgs/hero.bmp",
		16, 
		6, 5, 
		4, // amount of frames/imgs of walking animation
		8,  // amount of animation frames per second
		map->getCam()
		);
	NPCEntity->addComponent<DrawingComponent>(
		ren,
		"res/imgs/secondNPC.bmp",
		16, 
		3, 4,
		2,// amount of animation frames per second
		4, // amount of animation frames per second
		map->getCam()
		);

	playerEntity->addComponent<CollisionComponent>(map);
	NPCEntity->addComponent<CollisionComponent>(map);

	playerEntity->addComponent<InteractionComponent>(map);


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
			if (Event->key.repeat == 0) {
				switch (Event->key.keysym.sym) {
				case SDLK_w:
					w_keyDown = true;
					break;
				case SDLK_s:
					s_keyDown = true;
					break;
				case SDLK_a:
					a_keyDown = true;
					break;
				case SDLK_d:
					d_keyDown = true;
					break;
				}
			}
			break;
		case SDL_KEYUP:
			switch (Event->key.keysym.sym) {
			case SDLK_w:
				playerEntity->getComponent<PositionComponent>().setDirY_up(0);
				w_keyDown = false;
				break;
			case SDLK_s:
				playerEntity->getComponent<PositionComponent>().setDirY_down(0);
				s_keyDown = false;
				break;
			case SDLK_a:
				playerEntity->getComponent<PositionComponent>().setDirX_left(0);
				a_keyDown = false;
				break;
			case SDLK_d:
				playerEntity->getComponent<PositionComponent>().setDirX_right(0);
				d_keyDown = false;
				break;
			case SDLK_t:
				map->get_drawingSolidStates_bool() = map->get_drawingSolidStates_bool() ? false : true;
				break;
			case SDLK_l:
				// help key for debugging
				debugKeyDown = true;
				break;
			case SDLK_SPACE:
				playerEntity->getComponent<InteractionComponent>().interact();
				break;
			case SDLK_1:
				playerEntity->getComponent<DrawingComponent>().initCustomAnimation(4, 0, 6, 1, false);
				break;
			}
			break;
		}
	}
}

void GameEngine::handeKeyInputBools() {
	if (w_keyDown) playerEntity->getComponent<PositionComponent>().setDirY_up(1);
	if (s_keyDown) playerEntity->getComponent<PositionComponent>().setDirY_down(1);

	if (w_keyDown && s_keyDown) { 
		playerEntity->getComponent<PositionComponent>().setDirY_up(0); 
		playerEntity->getComponent<PositionComponent>().setDirY_down(0);
	}

	if (a_keyDown) playerEntity->getComponent<PositionComponent>().setDirX_left(1);
	if (d_keyDown) playerEntity->getComponent<PositionComponent>().setDirX_right(1);
	if (a_keyDown && d_keyDown) {
		playerEntity->getComponent<PositionComponent>().setDirX_left(0);
		playerEntity->getComponent<PositionComponent>().setDirX_right(0);
	}
}

 // raw manual code for npc movement
void GameEngine::test_NPCMoveFunction() {
	if (numberOfFramesSinceStart < 240) {
		NPCEntity->getComponent<PositionComponent>().setVel(4, 0);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, true);
	}
	else if (numberOfFramesSinceStart < 580) {
		NPCEntity->getComponent<PositionComponent>().setVel(-2.8f, 2.8f);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, false);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_DOWN, true);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, true);
	}
	else if (numberOfFramesSinceStart < 1100) {
		NPCEntity->getComponent<PositionComponent>().setVel(2.8f, 2.8f);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, true);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, false);
	}
	else if (numberOfFramesSinceStart < 1350) {
		NPCEntity->getComponent<PositionComponent>().setVel(-2.8f, 0);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_RIGHT, false);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_DOWN, false);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, true);
	}
	else if (numberOfFramesSinceStart < 1400) {
		NPCEntity->getComponent<PositionComponent>().setVel(0, 2.8f);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_DOWN, true);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_LEFT, false);
	}
	else {
		NPCEntity->getComponent<PositionComponent>().setVel(0, 0);
		NPCEntity->getComponent<PositionComponent>().setDir(DIR_DOWN, false);
	}
}

const void  GameEngine::test_portalAnimationFunction() {
	if (playerEntity->getComponent<DrawingComponent>().customAnimationHasEnded()
		&& playerEntity->getComponent<DrawingComponent>().getPrevCustomAnimationY() == 4) {
		playerEntity->getComponent<PositionComponent>().moveForward(2*TILE_SIZE_PIXELS);
	}
}

void GameEngine::update() {
	unStartElapsedTime = SDL_GetTicks();
	if (m_stateManager->get() == m_stateManager->state_gameRunning) {


		//
		handleEvents();

		handeKeyInputBools();

		// update all entities
		m_entityManager.update();
		playerEntity->getComponent<PositionComponent>().setAcc(0, 0);


		// move the npc
		test_NPCMoveFunction();


		// teleportation mechanic
		test_portalAnimationFunction();

		log(playerEntity->getComponent<PositionComponent>().getDir()[DIR_RIGHT]);

		//
		map->update(); 


		// 
		theaterEngine->update(numberOfFramesSinceStart);


		//
		draw();
	

		// delay
		nElapsedTime = SDL_GetTicks() - unStartElapsedTime;
		if (nDelay > nElapsedTime) {
			SDL_Delay(nDelay - nElapsedTime);
		}
		if ((SDL_GetTicks() - unStartElapsedTime) != 0) {
			actualFPS = 1000 / (SDL_GetTicks() - unStartElapsedTime);
		}
		std::string bufTitle = sTitle + "    FPS:" + std::to_string(actualFPS);
		SDL_SetWindowTitle(win, bufTitle.c_str());
	}

	numberOfFramesSinceStart++;
}

void GameEngine::sortEntityArray() {
	// array of all entities to be sorted
	arrayOfActiveEntities = m_entityManager.getEntityArrayPointer();
	// empty the buffer
	bufferArrayOfEntities.clear();
	// sort the arrayOfActiveEntities going highest y-value to lowest y-value !!!!! TODO
	while (arrayOfActiveEntities->size() > 0) {
		int suspectedIndex = 0;
		if (arrayOfActiveEntities->size() != 1) {
			for (int i = 1; i < arrayOfActiveEntities->size(); i++) {
				if (arrayOfActiveEntities->at(i)->hasComponent<PositionComponent>()) {
					if (arrayOfActiveEntities->at(i)->getComponent<PositionComponent>().gety() <
						arrayOfActiveEntities->at(suspectedIndex)->getComponent<PositionComponent>().gety()) {
						suspectedIndex = i;
					}
				}
			}
		}
		bufferArrayOfEntities.push_back(arrayOfActiveEntities->at(suspectedIndex));
		arrayOfActiveEntities->erase(arrayOfActiveEntities->begin() + suspectedIndex);
	}
	*arrayOfActiveEntities = bufferArrayOfEntities;
}

void GameEngine::renderText() {

	// this is the color in rgb format,
	// maxing out all would give you the color white,
	// and it will be your text's color
	SDL_Color White = { 255, 255, 255 };


	textMessage = "Broski";
	// as TTF_RenderText_Solid could only be used on
	// SDL_Surface then you have to create the surface first
	if (textMessage != lastMessage) {
		surfaceMessage =
			TTF_RenderText_Solid(arialFont, textMessage.c_str(), White);
		if (surfaceMessage == nullptr)log("surfaceMessage surface failed load!");
		// now you can convert it into a texture
		Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);
		if (Message == nullptr)log("Message tex failed load!");
		lastMessage = textMessage;
	}

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = 0;  //controls the text's x coordinate 
	Message_rect.y = 0; // controls the text's y coordinte


	TTF_SizeText(arialFont, "Broski", &Message_rect.w, &Message_rect.h);

	SDL_RenderCopy(ren, Message, NULL, &Message_rect);

	//SDL_FreeSurface(surfaceMessage);
	//SDL_DestroyTexture(Message);
}

void GameEngine::draw() {

	// clear screen
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderClear(ren);

	if (m_stateManager->get() == m_stateManager->state_gameRunning) {

		map->draw();
		
		// sort array in order to draw entities in front first
		sortEntityArray();
		m_entityManager.draw();

		map->drawSecondLayer();
	}

	renderText();

	if (ren == nullptr) log("sug min ljlævla fokicnig jcukuk brro");
	SDL_RenderPresent(ren);
}



bool GameEngine::alive() {
	return bRunning;
}

SDL_Renderer* GameEngine::getRen() const {
	return ren;
}

const uint32_t GameEngine::getWinSize() const {
	return nWinWidth, nWinHeight;
}
