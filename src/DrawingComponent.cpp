#include "DrawingComponent.h"


/*
SDL_Rect* destRect, * srcRect;
SDL_Renderer* ren;
spriteSheet* spriteSheet;
*/

#define ANIMATION_INCREMENT 0.1f
#define MAX_SPRITE_SIZE 2*AVERAGE_ENTITY_SIZE_PIXELS

DrawingComponent::DrawingComponent(
	SDL_Renderer* inputRen, // for drawing
	const std::string spriteTitle, // path of texture sheet
	const int walkSpriteWidthNHeight,
	const int spriteSheetColumns,
	const int spriteSheetRows,
	const int amountOfWalkingAnimationFrames,
	const unsigned int fps, // frames per state of animation
	const Camera* cam
	) {

	m_FPS = fps;

	// Load BMP into surface variable
	//SDL_Surface* spriteSheetSurface = SDL_LoadBMP(std::string("../res/imgs/" + spriteTitle).c_str());
	SDL_Surface* spriteSheetSurface = SDL_LoadBMP(spriteTitle.c_str());

	if (!spriteSheetSurface) {
		std::cout << SDL_GetError();
		log("SDL_Surface* spriteSheetSurface = SDL_LoadBMP(spriteTitle.c_str()); failed");
		exit(-1);
	}

	// set this renderer to window renderer and check if initialized
	m_ren = inputRen;
	m_cam = cam;

	if (!m_ren) {
		std::cout << "Renderer is nullptr!\n";
		exit(-1);
	}

	currentAnimationType = animationType::walkingAnimation;

	// init spritesheet ------------
	m_spriteSheet = new SpriteSheet{};

	// Turn surface into texture
	m_spriteSheet->tex = SDL_CreateTextureFromSurface(m_ren, spriteSheetSurface);
	if (m_spriteSheet->tex == nullptr) log("piss");

	// specify other spritesheet values
	m_spriteSheet->nWidth = spriteSheetColumns;
	m_spriteSheet->nHeight = spriteSheetRows;
	m_spriteSheet->nSize = walkSpriteWidthNHeight; // pixels i.e. here: 16x16

	// NULL all other private variables that should be NULLed
	m_destRect = new SDL_Rect{ 0, 0, AVERAGE_ENTITY_SIZE_PIXELS, AVERAGE_ENTITY_SIZE_PIXELS };
	m_srcRect = new SDL_Rect{ 0, 0, m_spriteSheet->nSize, m_spriteSheet->nSize };

	m_posComp = nullptr;

	startX = 0; 
	amountOfRepetitions = NULL;
	currentRepetition = 0;
	customAnimationHasEnded_notifier = false;
	prevCustomAnimationY = NULL;

	m_animationTick = 0;
	m_animationFramesPerState = amountOfWalkingAnimationFrames;
	m_framesPerImage = (int)(targetFPS / fps);
	m_frameCounter = 0;

	m_cameraOffsetX = m_cam->getOffsetXPtr();
	m_cameraOffsetY = m_cam->getOffsetYPtr();

	m_prevDir = 0;

	SDL_FreeSurface(spriteSheetSurface);

}

void DrawingComponent::init() {

	// Position component to entity's component
	if (ent->hasComponent<PositionComponent>()) {
		m_posComp = &ent->getComponent<PositionComponent>();
	}
	else {
		std::cout << "Entity doesn't have position component.\n";
		exit(-1);
	}
}

DrawingComponent::~DrawingComponent() {

}

const void DrawingComponent::initCustomAnimation(
	uint32_t customAnimationY, // of source image
	uint32_t startX,  // to start animation from
	uint32_t amountOfFrames, // for animation
	uint32_t amountOfRepetitions, // to be looped
	bool isAbleToMoveDuringAnimation
	) {
	// TODO: add assertions
	yValueBeforeCustomAnimation = m_srcRect->y;
	m_srcRect->y = customAnimationY * m_spriteSheet->nSize;
	prevCustomAnimationY = customAnimationY;
	m_animationTick = startX;
	this->startX = startX;
	this->amountOfFramesForCustomAnimation = amountOfFrames;
	this->amountOfRepetitions = amountOfRepetitions;
	this->isAbleToMoveDuringAnimation = isAbleToMoveDuringAnimation;
	currentRepetition = 0;
	currentAnimationType = animationType::customAnimation;
	ent->getComponent<PositionComponent>().set_isAbleToMove(isAbleToMoveDuringAnimation);
	customAnimationHasEnded_notifier = false;
}

void DrawingComponent::update() {
	switch (currentAnimationType) {
		case animationType::walkingAnimation:
			update_walkingAnimation();
			break;
		case animationType::customAnimation:
			update_customAnimation();
			break;
	}
}

void DrawingComponent::update_walkingAnimation() {
	if (m_posComp->isMoving()) { // if moving, update the animation frame
		if (m_animationTick == 0) m_animationTick = 1; // quick fix
		if (m_frameCounter >= m_framesPerImage) { // if image has been shown for correct amount of frames
			if (m_animationTick-1 < m_animationFramesPerState - 1) { // increment animationtick or reset if at limit. circular
				m_animationTick += 1;
			}
			else {
				m_animationTick = 1; // 1 because the spritesheet walking animation starts one over. idle is 0
			}
			m_frameCounter = 0;
		}
		else {
			m_frameCounter++;
		}
	}
	else {// if not moving, reset animation frame
		m_animationTick = 0;
		m_frameCounter = 0;
	}
}

void DrawingComponent::update_customAnimation() {
	if (currentRepetition < amountOfRepetitions) { // if not moving, update the animation frame
		if (m_frameCounter >= m_framesPerImage) { // if image has been shown for correct amount of frames
			if (m_animationTick < amountOfFramesForCustomAnimation - 1) { // increment animationtick or reset if at limit. circular
				m_animationTick += 1;
			}
			else {
				m_animationTick = startX;
				currentRepetition++;
			}
			m_frameCounter = 0;
		}
		else {
			m_frameCounter++;
		}
	} else { // if done with animation
		currentAnimationType = animationType::walkingAnimation;
		m_srcRect->y = yValueBeforeCustomAnimation;
		m_animationTick = 0;
		m_frameCounter = 0;
		if(!isAbleToMoveDuringAnimation) ent->getComponent<PositionComponent>().set_isAbleToMove(true);
		customAnimationHasEnded_notifier = true;
	}
}


const bool DrawingComponent::is_in_viewable_area() const {
	if (m_posComp->getx() + MAX_SPRITE_SIZE > * m_cameraOffsetX // m_cameraOffsetX being translateXcoordFromWinToMap(0)
		&& m_posComp->getx() < *m_cameraOffsetX + m_cam->getWinWidth()) {
		if (m_posComp->gety() + MAX_SPRITE_SIZE > * m_cameraOffsetY // m_cameraOffsetY being translateYcoordFromWinToMap(0)
			&& m_posComp->gety() < *m_cameraOffsetY + m_cam->getWinHeight()) {
			return true;
		}
	}
	return false;
}

void DrawingComponent::draw() {
	// set current position to position component's coords
	// m_cameraOffsetx/y can be seen as values of a vector describing translation from camera position to top-left corner of field of vision.
	//										vvvvvv     this will find the coordinates of the entity relative to the window based on the position in the "game world"
	m_destRect->x = m_posComp->getx() - *m_cameraOffsetX;
	m_destRect->y = m_posComp->gety() - *m_cameraOffsetY;

	// Update src rect by animation tick
	if (m_animationTick > m_spriteSheet->nWidth) log("m_animationTick > m_spriteSheet->nWidth");
	m_srcRect->x = m_spriteSheet->nSize * (int)(m_animationTick);

	if (currentAnimationType == animationType::walkingAnimation) {
		if(is_in_viewable_area()){
			draw_WalkingAnimationframe_according_to_direction();
		} else {
			updateWalkingAnimationWithoutDrawing_frame_according_to_direction();
		}
	} 
	else if(currentAnimationType == animationType::customAnimation) {
		if (is_in_viewable_area()) {
			draw_customAnimationFrame();
		}
	}
}

void DrawingComponent::draw_WalkingAnimationframe_according_to_direction() {
	// draw texture 
	// check direction and drawing accordingly
	// X
	// unsure if these get functions are inefficient :/
	if (m_posComp->isMovingX()) {
		m_srcRect->y = 0;
		if (m_posComp->getDir()[DIR_LEFT]) {
			SDL_RenderCopy(m_ren, m_spriteSheet->tex, m_srcRect, m_destRect);
			m_prevDir = DIR_LEFT;
		}
		else if (m_posComp->getDir()[DIR_RIGHT]) {
			SDL_RenderCopyEx(m_ren, m_spriteSheet->tex, m_srcRect, m_destRect, 0, 0, SDL_FLIP_HORIZONTAL);
			m_prevDir = DIR_RIGHT;
		}
	}		// Check for previous directions  
	// Y 
	else if (m_posComp->isMovingY()) {
		if (m_posComp->getDir()[DIR_UP]) {
			m_srcRect->y = m_spriteSheet->nSize;
			SDL_RenderCopy(m_ren, m_spriteSheet->tex, m_srcRect, m_destRect);
		}
		if (m_posComp->getDir()[DIR_DOWN]) {
			m_srcRect->y = 2 * m_spriteSheet->nSize;
			SDL_RenderCopy(m_ren, m_spriteSheet->tex, m_srcRect, m_destRect);
		}
	} else if (m_prevDir == DIR_RIGHT) {
		SDL_RenderCopyEx(m_ren, m_spriteSheet->tex, m_srcRect, m_destRect, 0, 0, SDL_FLIP_HORIZONTAL);
	} else {
	// in any other case where the entity is not moving and the previous direction is not right
	SDL_RenderCopy(m_ren, m_spriteSheet->tex, m_srcRect, m_destRect);
	}
}

void DrawingComponent::updateWalkingAnimationWithoutDrawing_frame_according_to_direction() {
	// check direction and drawing accordingly
	// unsure if these get functions are inefficient :/
	if (m_posComp->isMovingX()) {
		m_srcRect->y = 0;
	}
	else if (m_posComp->isMovingY()) {
		if (m_posComp->getDir()[DIR_UP]) {
			m_srcRect->y = m_spriteSheet->nSize;
		}
		if (m_posComp->getDir()[DIR_DOWN]) {
			m_srcRect->y = 2 * m_spriteSheet->nSize;
		}
	}
}

void DrawingComponent::draw_customAnimationFrame() {
	SDL_RenderCopy(m_ren, m_spriteSheet->tex, m_srcRect, m_destRect);
}

const bool DrawingComponent::customAnimationHasEnded() {
	if (customAnimationHasEnded_notifier) {
		customAnimationHasEnded_notifier = false;
		return true;
	}
	return false;
}
const int DrawingComponent::getPrevCustomAnimationY() const {
	return prevCustomAnimationY;
}