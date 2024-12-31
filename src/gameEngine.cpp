#include "GameEngine.h"

GameEngine::GameEngine(const uint32_t nWidth, const uint32_t nHeight, const std::string title, const uint32_t fps) {
	nElapsedTime = NULL;    // for capping fps - global
	unStartElapsedTime = 0;  // for capping fps - global
	nWinWidth = nWidth;
	nWinHeight = nHeight;
	bRunning = true;
	nFps = fps;
	targetFPS = fps; // quickfix - global variable 
	sTitle = title;
	nDelay = uint32_t(1000 / nFps);
	mousePos[0] = 0, mousePos[1] = 0;

	// for transition animation
	elapsedFrames = 0;

	// game state related stuff
	m_stateManagerPtr = new GameStateManager();

	m_stateManagerPtr->set(m_stateManagerPtr->state_startingScreen);

	// when game has fully started
	m_stateManagerPtr->set(m_stateManagerPtr->state_gameRunning);



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
	renPtr = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renPtr == 0) {
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

	gMusic = Mix_LoadMUS("res/audio/stanczyk.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		bRunning = false;
	}

	Mix_VolumeMusic(int((float)MIX_MAX_VOLUME * 0.5f));

	if (Mix_PlayingMusic() == 0)
	{
		//Play the music
		Mix_PlayMusic(gMusic, -1);
	}

	// entity manager

	// array of all entities to be sorted
	arrayOfActiveEntitiesPtr = m_entityManager.getEntityArrayPointer();

	// for cutscenes
	theaterEngine = new TheaterEngine();
	// load necessary maps
	// create necessary entities

	currentMapPtr = new GameMap*();

	theaterEngine->init(
		m_stateManagerPtr, 
		&m_entityManager,
		currentMapPtr,
		renPtr,
		&nWinWidth,
		&nWinHeight
		);

	playerEntity = theaterEngine->getPlayerEntity();
	if (playerEntity == nullptr) log("playerEntity == nullptr");

	/*
	NPCEntity=m_entityManager.addEntity();
	if (NPCEntity->hasComponent<PositionComponent>())log("NPCEntity->hasComponent<PositionComponent>");
	NPCEntity->addComponent<PositionComponent>(TILE_SIZE_PIXELS, TILE_SIZE_PIXELS);
	if (NPCEntity->hasComponent<DrawingComponent>())log("NPCEntity->hasComponent<DrawingComponent>");
	NPCEntity->addComponent<DrawingComponent>(
		renPtr,
		"res/imgs/secondNPC.bmp",
		16,
		3, 4,
		2,// amount of animation frames per second
		4, // amount of animation frames per second
		mapArray[currentMapId]->getCam()
		);

	if (NPCEntity->hasComponent<CollisionComponent>())log("NPCEntity->hasComponent<CollisionComponent>");
	NPCEntity->addComponent<CollisionComponent>(mapArray[currentMapId]);


	NPCEntity->getComponent<PositionComponent>().set_isFrictionless(true);
	NPCEntity->getComponent<PositionComponent>().set_default_acceleration(0);
	*/
}

GameEngine::~GameEngine() {
	SDL_DestroyRenderer(renPtr);
	SDL_DestroyWindow(win);
}

int a = 0;

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
			switch ((a % 6)) {
			case 0:
				log("outside door");
				break;
			case 1:
				break;
			case 2:
				log("outside door spawn");
				break;
			case 3:
				log("inside door");
				break;
			case 4:
				break;
			case 5:
				log("inside door spawn");
				break;
			}
			std::cout << "x: " << theaterEngine->getCurrentMap()->getCellx_fromCoord(mousePos[0] + *theaterEngine->getCurrentMap()->getCam()->getOffsetXPtr()) << "\n";
			std::cout << "y: " << theaterEngine->getCurrentMap()->getCelly_fromCoord(mousePos[1] + *theaterEngine->getCurrentMap()->getCam()->getOffsetYPtr()) << "\n";
			log("--------------");
			a++;
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
				(*currentMapPtr)->get_drawingSolidStates_bool() = (*currentMapPtr)->get_drawingSolidStates_bool() ? false : true;
				break;
			case SDLK_l:
				// help key for debugging
				debugKeyDown = true;
				break;
			case SDLK_SPACE:
				playerEntity->getComponent<InteractionComponent>().interact();
				//if (!changeCurrentMap(0)) changeCurrentMap(1);
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

const void  GameEngine::test_portalAnimationFunction() {
	if (playerEntity->getComponent<DrawingComponent>().customAnimationHasEnded()
		&& playerEntity->getComponent<DrawingComponent>().getPrevCustomAnimationY() == 4) {
		playerEntity->getComponent<PositionComponent>().moveForward(2*TILE_SIZE_PIXELS);
	}
}

void GameEngine::delayAndUpdateWindowTitle() {
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

void GameEngine::update() {
	unStartElapsedTime = SDL_GetTicks();
	if (m_stateManagerPtr->get() == m_stateManagerPtr->state_gameRunning) {

		//
		handleEvents();

		handeKeyInputBools();

		// update all entities
		m_entityManager.update();
		playerEntity->getComponent<PositionComponent>().setAcc(0, 0);

		// teleportation mechanic
		test_portalAnimationFunction();

		//
		(*currentMapPtr)->update();

		//
		draw();

		// 
		theaterEngine->update();

		// delay
		delayAndUpdateWindowTitle();
	} else if (m_stateManagerPtr->get() == m_stateManagerPtr->state_blockTransition) {
		(*currentMapPtr)->update();
		GameEngine::transitionDraw();
		delayAndUpdateWindowTitle();
	}
}

void GameEngine::sortEntityArray() {
	// empty the buffer
	bufferArrayOfEntities.clear();
	// sort the arrayOfActiveEntities going highest y-value to lowest y-value !!!!! TODO
	while (arrayOfActiveEntitiesPtr->size() > 0) {
		int suspectedIndex = 0;
		if (arrayOfActiveEntitiesPtr->size() != 1) {
			for (int i = 1; i < arrayOfActiveEntitiesPtr->size(); i++) {
				if (arrayOfActiveEntitiesPtr->at(i)->hasComponent<PositionComponent>()) {
					if (arrayOfActiveEntitiesPtr->at(i)->getComponent<PositionComponent>().gety() <
						arrayOfActiveEntitiesPtr->at(suspectedIndex)->getComponent<PositionComponent>().gety()) {
						suspectedIndex = i;
					}
				}
			}
		}
		bufferArrayOfEntities.push_back(arrayOfActiveEntitiesPtr->at(suspectedIndex));
		arrayOfActiveEntitiesPtr->erase(arrayOfActiveEntitiesPtr->begin() + suspectedIndex);
	}
	*arrayOfActiveEntitiesPtr = bufferArrayOfEntities;
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
		Message = SDL_CreateTextureFromSurface(renPtr, surfaceMessage);
		if (Message == nullptr)log("Message tex failed load!");
		lastMessage = textMessage;
	}

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = 0;  //controls the text's x coordinate 
	Message_rect.y = 0; // controls the text's y coordinte


	TTF_SizeText(arialFont, "Broski", &Message_rect.w, &Message_rect.h);

	SDL_RenderCopy(renPtr, Message, NULL, &Message_rect);

	//SDL_FreeSurface(surfaceMessage);
	//SDL_DestroyTexture(Message);
}

// main drawing for normal gameplay
void GameEngine::draw() {

	// clear screen
	SDL_SetRenderDrawColor(renPtr, 0, 0, 0, 255);
	SDL_RenderClear(renPtr);

	(*currentMapPtr)->draw();

	// sort array in order to draw entities in front last
	sortEntityArray();
	m_entityManager.draw();

	(*currentMapPtr)->drawSecondLayer();

	theaterEngine->draw();

	renderText();

	SDL_RenderPresent(renPtr);
}

constexpr int amountOfBoxesX = 10;
constexpr int amountOfBoxesY = 4;

// drawing to be done during transition
// incredibly sketchy and rough draft
// clean up later
void GameEngine::transitionDraw() {
	if (elapsedFrames == 0) {
		boxWidth = int(nWinWidth / amountOfBoxesX); // 20 -> amount of boxes to be drawn across window
		boxHeight = int(nWinHeight / amountOfBoxesY);
		drawRect = { 0, 0, boxWidth,boxHeight };
		boxCount = 0;
		SDL_SetRenderDrawColor(renPtr, 100, 100, 100, 255);
	}
	if (boxCount < (amountOfBoxesX+1) * amountOfBoxesY) {
		// transition animation
		drawRect.x = (boxCount % (amountOfBoxesX + 1)) * boxWidth;
		drawRect.y = int(boxCount / (amountOfBoxesX + 1)) * boxHeight;
		SDL_RenderFillRect(renPtr, &drawRect);
		boxCount++;
	}
	else if (boxCount < (amountOfBoxesX+1) * amountOfBoxesY * 2) {
		if(boxCount == (amountOfBoxesX + 1) * amountOfBoxesY)
			theaterEngine->initCurrentScene();
		draw();
		SDL_SetRenderDrawColor(renPtr, 100, 100, 100, 255);
		// transition animation in reverse
		drawRect.x = (boxCount % (amountOfBoxesX + 1)) * boxWidth;
		drawRect.y = int(boxCount / (amountOfBoxesX + 1)) * boxHeight;
		for (int i = boxCount - (amountOfBoxesX + 1) * amountOfBoxesY; i < (amountOfBoxesX + 1) * amountOfBoxesY; i++) {
			drawRect.x = (i % (amountOfBoxesX + 1)) * boxWidth;
			drawRect.y = int(i / (amountOfBoxesX + 1)) * boxHeight;
			SDL_RenderFillRect(renPtr, &drawRect);
		}
		SDL_RenderFillRect(renPtr, &drawRect);
		boxCount++;
	}
	else {
		m_stateManagerPtr->set(m_stateManagerPtr->state_gameRunning);
		elapsedFrames = -1; // because transitionDraw ends with adding 1
	}
	SDL_RenderPresent(renPtr);
	elapsedFrames++;
}

/*
const bool GameEngine::changeCurrentMap(GameMap* in_currentMapPtr) {
	if (in_currentMapPtr != nullptr) {
		for (Entity* entity : *arrayOfActiveEntitiesPtr) {
			if (entity->hasComponent<CollisionComponent>()) {
				entity->getComponent<CollisionComponent>().loadNewMap(in_currentMapPtr);
			}
			if (entity->hasComponent<DrawingComponent>()) {
				entity->getComponent<DrawingComponent>().loadNewCamera(in_currentMapPtr->getCam());
			}
			if (entity->hasComponent<PositionComponent>()) {
				entity->getComponent<PositionComponent>().setPos(TILE_SIZE_PIXELS*2, TILE_SIZE_PIXELS*3);
			}
		}
		currentMapPtr = in_currentMapPtr;
		return true;
	}
	return false;
}
*/


bool GameEngine::alive() {
	return bRunning;
}

SDL_Renderer* GameEngine::getRenPtr() const {
	return renPtr;
}

const uint32_t GameEngine::getWinSize() const {
	return nWinWidth, nWinHeight;
}
