#include "Camera.h"

#define X_OFFSET 300

Camera::Camera(int* x, int* y, const uint32_t mapWidth, const uint32_t mapHeight, const uint32_t winWidth, const uint32_t winHeight) {
	m_mapWidth = mapWidth;
	m_mapHeight = mapHeight;

	// because of vs
	m_winWidth = 0;
	m_winHeight = 0;

	m_winWidth = winWidth;
	m_winHeight = winHeight;
	m_xPos = x;
	m_yPos = y;
	m_xCameraOffset = 0;
	m_yCameraOffset = 0;
}

void Camera::update() {

	m_xCameraOffset = *m_xPos - X_OFFSET;
	m_yCameraOffset = *m_yPos - X_OFFSET;

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