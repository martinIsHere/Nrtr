#include "CollisionComponent.h"
#include "includes.h"
#define COLSN_OFST 0

CollisionComponent::CollisionComponent(GameMap* gameMap) {
	m_gameMap = gameMap;
	m_posComp = nullptr;
}

CollisionComponent::~CollisionComponent() {

}

void CollisionComponent::init() {
	if (!ent) {
		log("Component's entityPtr was nullptr!\n");
	}
	else {
		if (ent->hasComponent<PositionComponent>()) {

			m_posComp = &ent->getComponent<PositionComponent>();
		}
		else {
			m_posComp = nullptr;
			std::cout << "Entity does not have positionComponent!\n";
		}
	}
}



void CollisionComponent::update() {
	if (correct_possible_collision()) {
		//log("collision")
	}
}

void CollisionComponent::draw() {

}

inline bool CollisionComponent::apply_correction_in_LEFT_COLLISION(int oldYPos, int hitBoxOffsetX, int hitBoxTopOffset) {
	if (m_gameMap->getState(
		int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE_PIXELS),
		int((oldYPos + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE_PIXELS)
		)
		|| m_gameMap->getState(
			int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE_PIXELS),
			int(((oldYPos + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - 1) / TILE_SIZE_PIXELS)))
		) {

		// Correct collision: move back to the left boundary
		m_posComp->getx() =
			int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE_PIXELS) * TILE_SIZE_PIXELS + TILE_SIZE_PIXELS - hitBoxOffsetX;

		// Reset velocity to prevent movement into the collision
		m_posComp->getVelx() = 0;

		return true;
	}
	return false;
}

inline bool CollisionComponent::apply_correction_in_RIGHT_COLLISION(int oldYPos, int hitBoxOffsetX, int hitBoxTopOffset) {
	if (m_gameMap->getState(
		int((m_posComp->getx() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - hitBoxOffsetX - 1) / TILE_SIZE_PIXELS),
		int((oldYPos + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE_PIXELS))
		|| m_gameMap->getState(
			int(((m_posComp->getx() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - hitBoxOffsetX - 1) / TILE_SIZE_PIXELS)),
			int(((oldYPos + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - 1) / TILE_SIZE_PIXELS)))) {

		// Correct collision: move back to the right boundary
		m_posComp->getx() =
			int((m_posComp->getx() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - hitBoxOffsetX - 1) / TILE_SIZE_PIXELS) * TILE_SIZE_PIXELS - AVERAGE_ENTITY_SIZE_PIXELS + hitBoxOffsetX;

		// Reset velocity to prevent movement into the collision
		m_posComp->getVelx() = 0;

		//log("Collision right");
		return true;
	}
	return false;
}

inline bool CollisionComponent::apply_correction_in_ABOVE_COLLISION(int hitBoxOffsetX, int hitBoxTopOffset) {
	if (m_gameMap->getState(
		int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE_PIXELS),
		int((m_posComp->gety() + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE_PIXELS))
		|| m_gameMap->getState(
			int(((m_posComp->getx() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - hitBoxOffsetX - 1) / TILE_SIZE_PIXELS)),
			int((m_posComp->gety() + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE_PIXELS))) {

		// Correct collision: move back to the top boundary
		m_posComp->gety() =
			int((m_posComp->gety() + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE_PIXELS) * TILE_SIZE_PIXELS + TILE_SIZE_PIXELS - hitBoxTopOffset;

		// Reset velocity to prevent movement into the collision
		m_posComp->getVely() = 0;

		//log("Collision up");
		return true;
	}
	return false;
}

inline bool CollisionComponent::apply_correction_in_BELOW_COLLISION(int hitBoxOffsetX, int hitBoxTopOffset) {
	if (m_gameMap->getState(
		int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE_PIXELS),
		int((m_posComp->gety() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - 1) / TILE_SIZE_PIXELS))
		|| m_gameMap->getState(
			int(((m_posComp->getx() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - hitBoxOffsetX - 1) / TILE_SIZE_PIXELS)),
			int((m_posComp->gety() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - 1) / TILE_SIZE_PIXELS))) {

		// Correct collision: move back to the bottom boundary
		m_posComp->gety() =
			int((m_posComp->gety() + AVERAGE_ENTITY_SIZE_PIXELS - COLSN_OFST - 1) / TILE_SIZE_PIXELS) * TILE_SIZE_PIXELS - AVERAGE_ENTITY_SIZE_PIXELS;

		// Reset velocity to prevent movement into the collision
		m_posComp->getVely() = 0;

		//log("Collision down");
		return true;
	}
	return false;
}

bool CollisionComponent::correct_possible_collision() {

	bool collision = false;

	// may act up in the future... idk we'll see ------------------------------------------------------------
	// (int)m_posComp->getVely() is sketchy
	int oldYPos = int(m_posComp->gety() - (int)m_posComp->getVely());

	// Offsets for hitbox
	static int hitBoxOffsetX = int(AVERAGE_ENTITY_SIZE_PIXELS * 0.25f);
	static int hitBoxTopOffset = int(AVERAGE_ENTITY_SIZE_PIXELS * 0.5f);

	// TODO: check for collision with edges of map

	// X-axis collision handling
	if (m_posComp->isMovingVelX()) {
		if (m_posComp->isMovingLeftVel()) {
			// Check collision for top left and bottom left points and apply correction in case of collsion
			// collision is set to true if correction is needed
			// this check is done with previous y position
			collision = apply_correction_in_LEFT_COLLISION(oldYPos, hitBoxOffsetX, hitBoxTopOffset);
		}
		else if (m_posComp->isMovingRightVel()) {
			// Check collision for top right and bottom right points and apply correction in case of collsion
			// collision is set to true if correction is needed
			// this check is done with previous y position
			collision = apply_correction_in_RIGHT_COLLISION(oldYPos, hitBoxOffsetX, hitBoxTopOffset);
		}
	}

	// Y-axis collision handling
	if (m_posComp->isMovingVelY()) {
		if (m_posComp->isMovingUpVel()) {
			// Check collision for top left and top right points and apply correction in case of collsion
			// collision is set to true if correction is needed
			// this check is done with updated y position
			collision = apply_correction_in_ABOVE_COLLISION(hitBoxOffsetX, hitBoxTopOffset);
		}
		else if (m_posComp->isMovingDownVel()) {
			// Check collision for bottom left and bottom right points and apply correction in case of collsion
			// collision is set to true if correction is needed
			// this check is done with updated y position
			collision = apply_correction_in_BELOW_COLLISION(hitBoxOffsetX, hitBoxTopOffset);
		}
	}

	return collision;
}
