#include "CollisionComponent.h"

#define COLSN_OFST 1

CollisionComponent::CollisionComponent(GameMap* gameMap) {

	m_posComp = nullptr;

	if (gameMap) {
		m_gameMap = gameMap;
	}
	else {
		m_gameMap = nullptr;
		std::cout << "GameMap was nullptr\n";
	}

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
	correct_possible_collision();
}

void CollisionComponent::draw() {

}

bool CollisionComponent::correct_possible_collision() {

	int oldYPos = m_posComp->gety() - m_posComp->getVely();

	// offsets for hitbox
	static int hitBoxOffsetX = 5;
	static int hitBoxTopOffset = TILE_SIZE * 0.4;

	// X
	if (m_posComp->isMovingVelX()) {
		log("X")
		if (m_posComp->isMovingLeftVel()) {
			// check collision for top left and bottom left points
			if (m_gameMap->getState(
					int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE),
					int((oldYPos + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE)
				)
				|| 
				m_gameMap->getState(
					int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE),
					int(((oldYPos + TILE_SIZE - COLSN_OFST) / TILE_SIZE)) )
				)
			{

					// correct collision
					m_posComp->getx() = 
						int(m_posComp->getx()/TILE_SIZE)*TILE_SIZE
						+ TILE_SIZE - hitBoxOffsetX;

					// reset velocity
					m_posComp->getVelx() = 0;

					log("Collision left")
					return true;
			}

		}
		else if (m_posComp->isMovingRightVel()) {
			// check collision for top right and bottom right points
			if (m_gameMap->getState(
					int(((m_posComp->getx() + TILE_SIZE - COLSN_OFST - hitBoxOffsetX) / TILE_SIZE)),
					int((oldYPos + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE))
				||
				m_gameMap->getState(
					int(((m_posComp->getx() + TILE_SIZE - COLSN_OFST - hitBoxOffsetX) / TILE_SIZE)),
					int(((oldYPos + TILE_SIZE - COLSN_OFST) / TILE_SIZE))))
			{

					// correct collision
					m_posComp->getx() = 
						int(m_posComp->getx() / TILE_SIZE) * TILE_SIZE + hitBoxOffsetX;

					// reset velocity
					m_posComp->getVelx() = 0;

					log("Collision right")
					return true;
			}

		}
	}
	// Y
	if (m_posComp->isMovingVelY()) {
		log("Y")
		if (m_posComp->isMovingUpVel()) {
			// check collision for top left and top right points
			if (m_gameMap->getState(
					int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE),
					int((m_posComp->gety() + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE))
				||
				m_gameMap->getState(
					int(((m_posComp->getx() + TILE_SIZE - COLSN_OFST - hitBoxOffsetX) / TILE_SIZE)),
					int((m_posComp->gety() + COLSN_OFST + hitBoxTopOffset) / TILE_SIZE)))
			{

					// correct collision
					m_posComp->gety() =
						int(m_posComp->gety() / TILE_SIZE) * TILE_SIZE + TILE_SIZE - hitBoxTopOffset;

					// reset velocity
					m_posComp->getVely() = 0;

					log("Collision up")
					return true;
			}

		}
		else if (m_posComp->isMovingDownVel()) {
			// check collision for bottom left and bottom right points
			if (m_gameMap->getState(
					int((m_posComp->getx() + COLSN_OFST + hitBoxOffsetX) / TILE_SIZE),
					int((m_posComp->gety() + TILE_SIZE - COLSN_OFST) / TILE_SIZE))
				||
				m_gameMap->getState(
					int(((m_posComp->getx() + TILE_SIZE - COLSN_OFST - hitBoxOffsetX) / TILE_SIZE)),
					int((m_posComp->gety() + TILE_SIZE - COLSN_OFST) / TILE_SIZE)))
			{

					// correct collision
					m_posComp->gety() =
						int(m_posComp->gety() / TILE_SIZE) * TILE_SIZE;

					// reset velocity
					m_posComp->getVely() = 0;


					log("Collision down")
					return true;
			}

		}
	}
	return false;
}