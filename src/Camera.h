#pragma once
#include "includes.h"

class GameEngine;
class GameMap;

class Camera{

	int m_xCameraOffset, m_yCameraOffset;
	int* m_xPos, *m_yPos;
	uint32_t m_winWidth, m_winHeight;
	uint32_t m_mapWidth, m_mapHeight;

public:

	Camera(int* x, int* y, const uint32_t mapWidth, const uint32_t mapHeight, const uint32_t winWidth, const uint32_t winHeight);

	Camera() = default;
	
	void update();

	void update(const int& x, const int& y);

	int* getOffsetX();

	int* getOffsetY();

};

