#include "DrawingComponent.h"

/*
SDL_Rect* destRect, * srcRect;
SDL_Renderer* ren;
spriteSheet* spriteSheet;
*/

#define ANIMATION_INCREMENT 0.2f

DrawingComponent::DrawingComponent(SDL_Renderer* inputRen, const std::string spriteTitle, int spriteSheetColumns, int spriteSheetRows, const unsigned int fps, int* cameraOffsetX, int* cameraOffsetY) {

	m_FPS = fps;

	// Load BMP into surface variable
	//SDL_Surface* spriteSheetSurface = SDL_LoadBMP(std::string("../res/imgs/" + spriteTitle).c_str());
	SDL_Surface* spriteSheetSurface = SDL_LoadBMP("res/imgs/hero.bmp");

	if (!spriteSheetSurface) {
		std::cout << SDL_GetError();
		exit(-1);
	}

	// set this renderer to window renderer and check if initialized
	m_ren = inputRen;

	if (!m_ren) {
		std::cout << "Renderer is nullptr!\n";
		exit(-1);
	}

	// init spritesheet ------------
	m_spriteSheet = new SpriteSheet{};

	// Turn surface into texture
	m_spriteSheet->tex = SDL_CreateTextureFromSurface(m_ren, spriteSheetSurface);

	// specify other spritesheet values
	m_spriteSheet->nWidth = spriteSheetColumns;
	m_spriteSheet->nHeight = spriteSheetRows;
	m_spriteSheet->nSize = 16;

	// NULL all other private variables that should be NULLed
	m_destRect = new SDL_Rect{ 0, 0, 64, 64 };
	m_srcRect = new SDL_Rect{ 0, 0, m_spriteSheet->nSize, m_spriteSheet->nSize };

	m_posComp = nullptr;

	m_animationTick = 0.f;
	m_framesPerState = 3;
	m_animationTime = (float)m_FPS; // in ms
	m_currentFrame = 0;

	m_cameraOffsetX = cameraOffsetX;
	m_cameraOffsetY = cameraOffsetY;

	m_prevDir = 0;

	// vs is complaining
	m_maxFrameForAnimation = 0;
	m_maxFrameForAnimation = m_framesPerState - m_animationTickIncrement;
							// per
	m_animationTickIncrement = ANIMATION_INCREMENT;

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

	if (m_posComp->isMoving()) {
	if (m_animationTick < m_maxFrameForAnimation) {
		m_animationTick += m_animationTickIncrement;
	}
	else {
		m_animationTick = (float)m_currentFrame;
	}
	}
	else {
		m_animationTick = (float)m_currentFrame;
	}
}

void DrawingComponent::draw() {
	// set current position to position component's coords
	m_destRect->x = m_posComp->getx() - *m_cameraOffsetX;
	m_destRect->y = m_posComp->gety() - *m_cameraOffsetY;

	// Update src rect by animation tick
	m_srcRect->x = m_spriteSheet->nSize * ((int)m_animationTick % m_spriteSheet->nWidth);

	// image specific stuff: ------------------------------------------
	/*
	The source rect stuff position
	*/
	// -----------------------------------------------------------------

	draw_frame_according_to_direction();

}

void DrawingComponent::draw_frame_according_to_direction() {
	// draw texture 
	// check direction and drawing accordingly
	// X
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
	}
	else if (m_prevDir == DIR_LEFT) {
		SDL_RenderCopy(m_ren, m_spriteSheet->tex, m_srcRect, m_destRect);
	}
	else if (m_prevDir == DIR_RIGHT) {
		SDL_RenderCopyEx(m_ren, m_spriteSheet->tex, m_srcRect, m_destRect, 0, 0, SDL_FLIP_HORIZONTAL);
	}
	else if (m_prevDir == DIR_UP) {
		SDL_RenderCopy(m_ren, m_spriteSheet->tex, m_srcRect, m_destRect);
	}
	else if (m_prevDir == DIR_DOWN) {
		SDL_RenderCopy(m_ren, m_spriteSheet->tex, m_srcRect, m_destRect);
	}
}