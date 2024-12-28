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
	const unsigned int fps,
	const Camera* cam
	//int* cameraOffsetX, // from camera class
	//int* cameraOffsetY
	) {

	m_FPS = fps;

	// Load BMP into surface variable
	//SDL_Surface* spriteSheetSurface = SDL_LoadBMP(std::string("../res/imgs/" + spriteTitle).c_str());
	SDL_Surface* spriteSheetSurface = SDL_LoadBMP(spriteTitle.c_str());

	if (!spriteSheetSurface) {
		std::cout << SDL_GetError();
		exit(-1);
	}

	// set this renderer to window renderer and check if initialized
	m_ren = inputRen;
	m_cam = cam;

	if (!m_ren) {
		std::cout << "Renderer is nullptr!\n";
		exit(-1);
	}

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

	m_animationTick = 0.f;
	m_animationFramesPerState = m_spriteSheet->nWidth;
	m_framesPerImage = 15;
	m_animationTime = (float)m_FPS; // in ms
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

void DrawingComponent::update() {
	if (m_posComp->isMoving()) { // if moving, update the animation frame
		if (m_animationTick == 0) m_animationTick = 1;
		if (m_frameCounter >= m_framesPerImage) { // if image has been shown for correct amount of frames
			if (m_animationTick < m_animationFramesPerState-1) { // increment animationtick or reset if at limit. circular
				m_animationTick += 1;
			} else {
				m_animationTick = 1;
			}
			m_frameCounter = 0;
		} else {
			m_frameCounter++;
		}
	} else {// if not moving, reset animation frame
		m_animationTick = 0;
		m_frameCounter = 0;
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

	if(is_in_viewable_area()){
		draw_frame_according_to_direction();
	} else {
		updateWithoutDrawing_frame_according_to_direction();
	}
}

void DrawingComponent::draw_frame_according_to_direction() {
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

void DrawingComponent::updateWithoutDrawing_frame_according_to_direction() {
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