#include "CollisionComponent.h"

CollisionComponent::CollisionComponent(GameMap* gameMap) {
	if (ent->hasComponent<PositionComponent>()) {
		posComp = &ent->getComponent<PositionComponent>();
	}
	else {
		posComp = nullptr;
		std::cout << "Entity does not have positionComponent!\n";
	}

}

CollisionComponent::~CollisionComponent() {

}

void CollisionComponent::init() {

}

void CollisionComponent::update() {

}

void CollisionComponent::draw() {

}

bool CollisionComponent::correct_possible_collision() {
	return false;
	// X
	if (posComp->isMovingX()) {
		if (posComp->getDir()[DIR_LEFT]) {
			// check collision for top left and bottom left point

			if (1) {

			}

		}
		else if (posComp->getDir()[DIR_RIGHT]) {

		}
	}
	// Y
	else if (posComp->isMovingY()) {
		if (posComp->getDir()[DIR_UP]) {

		}
		else if (posComp->getDir()[DIR_DOWN]) {

		}
	}
}