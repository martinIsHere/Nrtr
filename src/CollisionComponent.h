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


private:

	// true = entity has collided
	bool correct_possible_collision();

	// true = entity has collided
	inline bool apply_correction_in_LEFT_COLLISION(int oldYPos, int hitBoxOffsetX, int hitBoxTopOffset);

	// true = entity has collided
	inline bool apply_correction_in_RIGHT_COLLISION(int oldYPos, int hitBoxOffsetX, int hitBoxTopOffset);

	// true = entity has collided
	inline bool apply_correction_in_ABOVE_COLLISION(int hitBoxOffsetX, int hitBoxTopOffset);

	// true = entity has collided
	inline bool apply_correction_in_BELOW_COLLISION(int hitBoxOffsetX, int hitBoxTopOffset);


};

