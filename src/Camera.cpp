#include "Camera.h"

Camera::Camera(int* x, int* y, const uint32_t mapWidth, const uint32_t mapHeight, const uint32_t winWidth, const uint32_t winHeight) {
	m_mapWidth = mapWidth;
	m_mapHeight = mapHeight;

	m_winWidth = winWidth;
	m_winHeight = winHeight;
	m_xPos = x;
	m_yPos = y;
	m_xCameraOffset = 0;
	m_yCameraOffset = 0;
	m_xOffsetFromScreenEdge = (int)((float)winWidth * 0.5f);
	m_yOffsetFromScreenEdge = (int)((float)winHeight * 0.5f);
}

void Camera::update() {

	m_xCameraOffset = *m_xPos - m_xOffsetFromScreenEdge;
	m_yCameraOffset = *m_yPos - m_yOffsetFromScreenEdge;

	// check if offset is behind 0
	
	if (m_xCameraOffset < 0) {
		m_xCameraOffset = 0;
	}
	if (m_yCameraOffset < 0) {
		m_yCameraOffset = 0;
	}
	

	// check if offset + screen size is more than the map's length
	
	if (m_xCameraOffset > int((m_mapWidth * TILE_SIZE) - m_winWidth)) {
		m_xCameraOffset = (m_mapWidth * TILE_SIZE) - m_winWidth;
	}
	if (m_yCameraOffset > int((m_mapHeight * TILE_SIZE) - m_winHeight)) {
		m_yCameraOffset = (m_mapHeight * TILE_SIZE) - m_winHeight;
	}
	
}

int* Camera::getOffsetX() {
	return &m_xCameraOffset;
}

int* Camera::getOffsetY() {
	return &m_yCameraOffset;
}