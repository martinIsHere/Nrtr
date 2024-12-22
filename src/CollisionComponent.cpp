#include "CollisionComponent.h"
#include "includes.h"
#define COLSN_OFST 0

CollisionComponent::CollisionComponent(GameMap* gameMap) {
	m_gameMap = gameMap;
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

bool CollisionComponent::correct_possible_collision() {
	/*
	log("------------------------------");
	log("start info:");
	std::cout << m_posComp->getx() << ", " << m_posComp->gety() << "\n";
	if (m_posComp->isMovingLeftVel()) log("Left");
	if (m_posComp->isMovingRightVel()) log("Right");
	if (m_posComp->isMovingUpVel()) log("Up");
	if (m_posComp->isMovingDownVel()) log("Down");
	log("correct_possible_collision: ");
	*/
	bool collision = false;

	int oldYPos = int(m_posComp->gety() - (int)m_posComp->getVely());

	// Offsets for hitbox
	static int hitBoxOffsetX = 15;
	static int hitBoxTopOffset = int(TILE_SIZE * 0.5);

	// Buffer positions
	int bufferX = m_posComp->getx();
	int bufferY = m_posComp->gety();

	// X-axis collision handling
	if (m_posComp->isMovingVelX()) {
		if (m_posComp->isMovingLeftVel()) {
			// Check collision for top left and bottom left points
			if (m_gameMap->getState(
				int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE),
				int((oldYPos + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE)
				)
				|| m_gameMap->getState(
					int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE),
					int(((oldYPos + TILE_SIZE - COLSN_OFST - 1) / TILE_SIZE)))
				) {

				// Correct collision: move back to the left boundary
				m_posComp->getx() =
					(int(m_posComp->getx()) / TILE_SIZE) * TILE_SIZE + TILE_SIZE - hitBoxOffsetX;

				// Reset velocity to prevent movement into the collision
				m_posComp->getVelx() = 0;

				//log("Collision left");
				collision = true;
			}
		}
		else if (m_posComp->isMovingRightVel()) {
			// Check collision for top right and bottom right points
			if (m_gameMap->getState(
				int(((m_posComp->getx() + TILE_SIZE - COLSN_OFST - hitBoxOffsetX - 1) / TILE_SIZE)),
				int((oldYPos + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE))
				|| m_gameMap->getState(
					int(((m_posComp->getx() + TILE_SIZE - COLSN_OFST - hitBoxOffsetX - 1) / TILE_SIZE)),
					int(((oldYPos + TILE_SIZE - COLSN_OFST - 1) / TILE_SIZE)))) {

				// Correct collision: move back to the right boundary
				m_posComp->getx() =
					(int(m_posComp->getx()) / TILE_SIZE) * TILE_SIZE + hitBoxOffsetX;

				// Reset velocity to prevent movement into the collision
				m_posComp->getVelx() = 0;

				//log("Collision right");
				collision = true;
			}
		}
	}

	// Y-axis collision handling
	if (m_posComp->isMovingVelY()) {
		if (m_posComp->isMovingUpVel()) {
			// Check collision for top left and top right points
			if (m_gameMap->getState(
				int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE),
				int((m_posComp->gety() + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE))
				|| m_gameMap->getState(
					int(((m_posComp->getx() + TILE_SIZE - COLSN_OFST - hitBoxOffsetX - 1) / TILE_SIZE)),
					int((m_posComp->gety() + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE))) {

				// Correct collision: move back to the top boundary
				m_posComp->gety() =
					(int(m_posComp->gety()) / TILE_SIZE) * TILE_SIZE + TILE_SIZE - hitBoxTopOffset;

				// Reset velocity to prevent movement into the collision
				m_posComp->getVely() = 0;

				//log("Collision up");
				collision = true;
			}
		}
		else if (m_posComp->isMovingDownVel()) {
			// Check collision for bottom left and bottom right points
			if (m_gameMap->getState(
				int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE),
				int((m_posComp->gety() + TILE_SIZE - COLSN_OFST - 1) / TILE_SIZE))
				|| m_gameMap->getState(
					int(((m_posComp->getx() + TILE_SIZE - COLSN_OFST - hitBoxOffsetX - 1) / TILE_SIZE)),
					int((m_posComp->gety() + TILE_SIZE - COLSN_OFST - 1) / TILE_SIZE))) {

				// Correct collision: move back to the bottom boundary
				m_posComp->gety() =
					(int(m_posComp->gety()) / TILE_SIZE) * TILE_SIZE;

				// Reset velocity to prevent movement into the collision
				m_posComp->getVely() = 0;

				//log("Collision down");
				collision = true;
			}
		}
	}
	/*
	log("End info:");
	std::cout << m_posComp->getx() << ", " << m_posComp->gety() << "\n";
	if (m_posComp->isMovingLeftVel()) log("Left");
	if (m_posComp->isMovingRightVel()) log("Right");
	if (m_posComp->isMovingUpVel()) log("Up");
	if (m_posComp->isMovingDownVel()) log("Down");
	*/
	return collision;
}
