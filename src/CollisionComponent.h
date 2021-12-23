#pragma once
#include "ECS.h"
#include "PositionComponent.h"
#include "gameMap.h"

class CollisionComponent : public Component{

	PositionComponent* m_posComp;

	GameMap* m_gameMap;
	
public:

	CollisionComponent(GameMap* gameMap);

	~CollisionComponent();

	void init() override;

	void update() override;

	void draw() override;

	// true = entity has collided, false = no collision
	bool correct_possible_collision();

private:



};

