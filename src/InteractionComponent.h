#pragma once
#include "ECS.h"
#include "PositionComponent.h"
#include "DrawingComponent.h"
#include "gameMap.h"

class InteractionComponent : public Component {

	PositionComponent* m_posComp;

	GameMap* m_gameMap;

	bool m_hasInteracted;

public:

	void interact();

	InteractionComponent(GameMap* gameMap);

	~InteractionComponent();

	void init() override;

	void update() override;

	void draw() override;

private:


};

