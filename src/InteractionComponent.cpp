#include "InteractionComponent.h"

InteractionComponent::InteractionComponent(GameMap* gameMap) {
	m_gameMap = gameMap;
	m_posComp = nullptr;
	m_hasInteracted = false;
}



void InteractionComponent::init() {
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
InteractionComponent::~InteractionComponent() {

}

void interFunc() {
	log("cancer");
}

void InteractionComponent::update() {

	// creating coordinates in a different unit size - blocksized
	int bs = m_gameMap->getBlockSize();
	//int offsetBS = int(bs * 0.5f);
	float px = ((float)m_posComp->getx() / (float)bs);
	float py = ((float)m_posComp->gety() / (float)bs);
	if (m_hasInteracted) {
		if (m_posComp->getFacingDir() == DIR_RIGHT) {
			int touched_ID = m_gameMap->getBackID(int(px + 1), int(py + 0.5f)); // block that has been interacted with
			
			interFunc();
		}

	}
	m_hasInteracted = false;
}

void InteractionComponent::draw() {

}

void InteractionComponent::interact() {
	m_hasInteracted = true;
}