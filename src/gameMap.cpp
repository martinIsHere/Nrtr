#include "GameMap.h"


/*

Map structure

2 bytes - map width
2 bytes - map height

repeating:
	
	2 bytes frontLayer id
	2 bytes backLayer id
	1 byte: 
	7
	6
	5
	4 = mirrorState 2 - back
	3 = mirrorState 2
	2 = mirrorState 1 - front
	1 = mirrorState 1
	0 = solidState

	mirrorState:
	0 = nothing
	1 = vertical mirroring
	2 = horizontal mirroring
	3 = both
*/

void GameMap::loadMap(std::string map) {
	
	m_mapFile.open(map, std::fstream::in | std::fstream::binary);

	// get first 2 bytes
	m_mapFile.read(m_readingBuffer, 2);
	m_mapWidth = m_readingBuffer[1] & 0xFFFF;


	// get 2 bytes for height
	m_mapFile.read(m_readingBuffer, 2);
	m_mapHeight = m_readingBuffer[1] & 0xFFFF;

	//m_frontLayer_array = new uint16_t[m_mapWidth*m_mapHeight];
	//m_backLayer_array = new uint16_t[m_mapWidth*m_mapHeight];
	//m_blockState_array = new bool[m_mapWidth * m_mapHeight];
	
	for (uint32_t y = 0; y < m_mapHeight; y++) {
		for (uint32_t x = 0; x < m_mapWidth; x++) {
			
			// front id
			m_mapFile.read(m_readingBuffer, 2);
			m_current_ID = (m_readingBuffer[0] & 0xFF)<<8;
			m_current_ID += m_readingBuffer[1] & 0xFF;
			m_frontLayer_array.push_back(m_current_ID);
			//m_frontLayer_array[x*y] = m_current_ID;

			// back id
			m_mapFile.read(m_readingBuffer, 2);
			m_current_ID = (m_readingBuffer[0] & 0xFF) << 8;
			m_current_ID += m_readingBuffer[1] & 0xFF;
			m_backLayer_array.push_back(m_current_ID);
			//m_backLayer_array[x * y] = m_current_ID;


			// TODO : add mirror bit
			// state id
			m_mapFile.read(m_readingBuffer, 1);

			m_currentState = m_readingBuffer[0] & 0b1;
			m_blockState_array.push_back(m_currentState == 1);
			
			m_currentMirrorState = (uint8_t)((m_readingBuffer[0] >> 1) & 0b11);
			m_backMirrorState_array.push_back(m_currentMirrorState);

			m_currentMirrorState = (uint8_t)((m_readingBuffer[0] >> 3) & 0b11);
			m_frontMirrorState_array.push_back(m_currentMirrorState);
			
			//m_blockState_array[x * y] = m_currentState;

		}
	}
	
	m_mapFile.close();
}

GameMap::GameMap(SDL_Renderer* ren, const std::string map, const uint32_t windowWidth, const uint32_t windowHeight, int *x, int *y) {
	
	playerX = x;
	playerY = y;
	m_ren = ren;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_xId = 0;
	m_yId = 0;
	m_current_ID = 0;
	m_currentState = 0;
	m_blockSize = TILE_SIZE;
	m_ren = m_ren;
	m_tempSrcRect = new SDL_Rect;
	m_tempDstRect = new SDL_Rect;

	//load spriteSheet
	
	m_spriteSheet0 = new SpriteSheet{SDL_CreateTextureFromSurface(m_ren, SDL_LoadBMP("res/imgs/sh1.bmp")),  16, 12, 18};
	if (!m_spriteSheet0->tex) log("Failed to load texture.");

	// load map
	loadMap(map);

	m_mainCamera = new Camera(
		playerX, playerY, // position
		m_mapWidth, m_mapHeight, // map size
		m_windowWidth, m_windowHeight // window size
		);

}



/*
//temp   = SDL_LoadBMP("sprite.bmp");
sprite = SDL_DisplayFormat(SDL_LoadBMP("sprite.bmp"));
SDL_FreeSurface(temp);
*/

GameMap::~GameMap() {
	delete m_spriteSheet0, m_tempSrcRect, m_tempDstRect, m_mainCamera;
}

void GameMap::update() {
	m_mainCamera->update();;
}

int mousePositionX;
int mousePositionY;

SDL_Rect* dstRect2 = new SDL_Rect;

void GameMap::draw() {

	int offsetX = *m_mainCamera->getOffsetX();
	int offsetY = *m_mainCamera->getOffsetY();

	SDL_GetMouseState(&mousePositionX, &mousePositionY);
	mousePositionX += offsetX;
	mousePositionY += offsetY;

	//log((int)getBackMirrorState(mousePositionX / TILE_SIZE, mousePositionY / TILE_SIZE))

	dstRect2->x = int(mousePositionX/TILE_SIZE)*TILE_SIZE - offsetX;
	dstRect2->y = int(mousePositionY/TILE_SIZE)* TILE_SIZE - offsetY,
	dstRect2->w = TILE_SIZE;
	dstRect2->h = TILE_SIZE;

	for (uint32_t y = offsetY / m_blockSize; y < (m_windowHeight + offsetY) / m_blockSize + 1; y++) {
		for (uint32_t x = offsetX / m_blockSize; x < (m_windowWidth + offsetX) / m_blockSize + 1; x++) {


			*m_tempDstRect = { 
				int((x * m_blockSize) - offsetX),
				int((y * m_blockSize) - offsetY),
				int(m_blockSize),
				int(m_blockSize) 
			};

			// back layer
			int id = getBackID(x, y);
			if (id != 0) {
				m_xId = id % m_spriteSheet0->nWidth;
				m_yId = (int)(id / m_spriteSheet0->nWidth);
				*m_tempSrcRect = {
					m_xId* m_spriteSheet0->nSize,
					m_yId* m_spriteSheet0->nSize,
					m_spriteSheet0->nSize, 
					m_spriteSheet0->nSize
				};
				
				SDL_RenderCopyEx(
					m_ren,
					m_spriteSheet0->tex,
					m_tempSrcRect, m_tempDstRect,
					0, 0,
					(SDL_RendererFlip)getBackMirrorState(x, y)
					);
				
				
			}

			// draw frontlayer
			id = getFrontID(x, y);
			if (id != 0) {
				m_xId = id % m_spriteSheet0->nWidth;
				m_yId = (int)(id / m_spriteSheet0->nWidth);
				*m_tempSrcRect = {
					m_xId * m_spriteSheet0->nSize, 
					m_yId * m_spriteSheet0->nSize,
					m_spriteSheet0->nSize,
					m_spriteSheet0->nSize 
				};

				SDL_RenderCopyEx(
					m_ren, 
					m_spriteSheet0->tex,
					m_tempSrcRect, m_tempDstRect, 
					0, 0, 
					(SDL_RendererFlip)getFrontMirrorState(x, y)
					);
				
				
			}
			if (m_isDrawingSolidStates) {
				if (getState(x, y)) {
					SDL_SetRenderDrawColor(m_ren, 255, 0, 0, 200);
					SDL_RenderDrawRect(m_ren, m_tempDstRect);
				}
			}

		}
	}

	SDL_SetRenderDrawColor(m_ren, 0, 255, 255, 255);
	SDL_RenderDrawRect(m_ren, dstRect2);
	
}

uint16_t GameMap::getBackID(int x, int y) {
	if(x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		return m_backLayer_array[(size_t(y) * m_mapWidth) + x];
	return 0;
}
void GameMap::setBackID(int x, int y, int ID) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		m_backLayer_array[(size_t(y) * m_mapWidth) + x] = ID;
}
uint16_t GameMap::getFrontID(int x, int y) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		return m_frontLayer_array[(size_t(y) * m_mapWidth) + x];
	return 0;
}
void GameMap::setFrontID(int x, int y, int ID) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		m_frontLayer_array[(size_t(y) * m_mapWidth) + x] = ID;
}
bool GameMap::getState(int x, int y) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		return m_blockState_array[(size_t(y) * m_mapWidth) + x];
	return 0;
}
void GameMap::setState(int x, int y, bool state) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		m_blockState_array[(size_t(y) * m_mapWidth) + x] = state;
}
uint8_t& GameMap::getBackMirrorState(int x, int y) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		return m_backMirrorState_array[(size_t(y) * m_mapWidth) + x];
	else {
		return m_backMirrorState_array[0];
		log("getBackMirrorState out of range");
	}
}
uint8_t& GameMap::getFrontMirrorState(int x, int y) {
	if (x >= 0 && x < int(m_mapWidth) && y >= 0 && y < int(m_mapHeight))
		return m_frontMirrorState_array[(size_t(y) * int(m_mapWidth)) + x];
	else {
		return m_frontMirrorState_array[0];
		log("getBackMirrorState out of range");
	}
}
uint32_t& GameMap::getBlockSize() {
	return m_blockSize;
}

Camera* GameMap::getCam() {
	return m_mainCamera;
}

bool& GameMap::get_drawingSolidStates_bool() {
	return m_isDrawingSolidStates;
}