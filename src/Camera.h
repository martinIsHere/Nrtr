#pragma once
#include "includes.h"

class GameEngine;
class GameMap;

class Camera{

	int m_xCameraOffset, m_yCameraOffset;
	int* m_xPos, *m_yPos;
	int m_xOffsetFromScreenEdge, m_yOffsetFromScreenEdge;
	uint32_t m_winWidth, m_winHeight;
	uint32_t m_mapWidth, m_mapHeight;
	uint32_t m_visibleTilesX, m_visibleTilesY;
	bool m_isSmallMap;

public:

	Camera(
		int* x, 
		int* y, 
		const uint32_t mapWidth, 
		const uint32_t mapHeight, 
		const uint32_t winWidth, 
		const uint32_t winHeight,
		bool isSmallMap
		);

	Camera() = default;
	
	void update();

	void update(const int& x, const int& y);

	const uint32_t& getVisibleTilesX() const;

	const uint32_t& getVisibleTilesY() const;

	const int* getOffsetXPtr()const;

	const int* getOffsetYPtr()const;

	uint32_t getWinWidth()const;

	uint32_t getWinHeight()const;

	int translateXcoordFromWinToMap(const int x) const;

	int translateYcoordFromWinToMap(const int y) const;
};

