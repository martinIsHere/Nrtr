#pragma once

#include "ECS.h"
#include "includes.h"


/*
Will handle an entity's position on the screen. Accompanying this component will be a drawing component that will use
the coordinates to draw a sprite at that location. Very simple

I look forward to creating these components.
*/

class PositionComponent : public Component{

	int m_X, m_Y;
	Vector *m_velocity_vector;
	Vector *m_acceleration_vector;
	float m_Magnitude;
	std::array<bool, 4> m_Directions;
	float m_speed;
	float m_deltaTime_speedFix;

	float MAX_VEL;
	float NATURAL_DECELERATION;
	float DEFAULT_SPEED;
	float DEFAULT_NATURAL_DECELERATION;

public:

	PositionComponent(int x, int y);

	PositionComponent() = default;

	~PositionComponent();

	void init() override;

	int& getx();

	int& gety();
	
	int getPos() const;

	float getMaxVel() const;

	void update() override;

	void draw() override;

	void normalize();

	void setAcc(const float& xAcc, const float& yAcc);
	
	void setDirX(const bool& left, const bool& right);

	void setDirY(const bool& up, const bool& down);

	void setDirX_left(const bool& dir);

	void setDirX_right(const bool& dir);

	void setDirY_up(const bool& dir);

	void setDirY_down(const bool& dir);

	bool isMoving();

	bool isMovingX();

	bool isMovingY();

	float getVelMagnitude();

	std::array<bool, 4>& getDir();

private: 

	void apply_natural_deceleration();

	void update_current_acceleration();

	void cap_velocity();

	void setVel(const int x, const int y);

	void set_deltaTime_speedFix();

};
