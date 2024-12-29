#pragma once
#include "includes.h"
#include "gameEngine.h"
#include "Camera.h"


class GameEngine;

class GameMap {
public:
	GameMap(SDL_Renderer* ren,
		const std::string map, 
		const uint32_t windowWidth, 
		const uint32_t windowHeight,
		int* cameraPosx, 
		int* cameraPosy
		);
	
	~GameMap();
	
	void loadMap(std::string map);

	void draw();

	void drawSecondLayer();

	void update();

	const uint32_t& getMapWidth_tiles();

	const uint32_t& getMapHeight_tiles();

	uint16_t getForegroundID(int x, int y);

	void setForegroundID(int x, int y, int id);

	uint16_t getBackID(int x, int y);

	void setBackID(int x, int y, int id);

	uint16_t getFrontID(int x, int y);

	void setFrontID(int x, int y, int id);

	bool getState(int x, int y);

	void setState(int x, int y, bool state);

	uint8_t& getBackMirrorState(int x, int y);

	uint8_t& getFrontMirrorState(int x, int y);

	uint8_t& getForegroundMirrorState(int x, int y);

	uint32_t& getBlockSize();

	Camera* getCam();

	bool& get_drawingSolidStates_bool();

	const int getXcoordOnscreen_fromMapcoord(const int x)const;

	const int getYcoordOnscreen_fromMapcoord(const int y)const;

private:

	std::vector <uint16_t> m_backLayer_array;
	std::vector <uint16_t> m_frontLayer_array;
	std::vector <uint16_t> m_foregroundLayer_array;
	std::vector <bool> m_blockState_array;
	std::vector <uint8_t> m_foregroundMirrorState_array;
	std::vector <uint8_t> m_frontMirrorState_array;
	std::vector <uint8_t> m_backMirrorState_array;

	//uint16_t* m_backLayer_array;
	//uint16_t* m_frontLayer_array;
	//bool* m_blockState_array;

	GameEngine* m_gameEngine_instance;
	SDL_Renderer* m_ren;

	SDL_Rect* m_tempSrcRect, * m_tempDstRect;

	uint16_t m_mapWidth, m_mapHeight;
	uint32_t m_windowWidth, m_windowHeight;
	uint16_t m_current_ID, m_xId, m_yId;
	uint8_t m_currentState;
	uint8_t m_currentMirrorState;
	uint32_t m_blockSize;
	Camera* m_mainCamera;

	char m_readingBuffer[2];

	bool m_isDrawingSolidStates;

	std::ifstream m_mapFile;

	SpriteSheet* m_spriteSheet0;
};