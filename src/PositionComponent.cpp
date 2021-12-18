#include "PositionComponent.h"


PositionComponent::PositionComponent(int x, int y) : m_X(x), m_Y(y) {
	m_velocity_vector = nullptr;
	m_acceleration_vector = nullptr;
	m_Magnitude = 0;
	m_Directions = { false, false, false, false };
	DEFAULT_SPEED = 1.f;
	DEFAULT_NATURAL_DECELERATION = 0.5f;
	m_speed = DEFAULT_SPEED;
	NATURAL_DECELERATION = DEFAULT_NATURAL_DECELERATION;
	MAX_VEL = 10;
	m_deltaTime_speedFix = 0;
}

void PositionComponent::init() {
	// init
	m_velocity_vector = new Vector(0.f, 0.f);
	m_acceleration_vector = new Vector(0.f, 0.f);

}

PositionComponent::~PositionComponent() {
}

int& PositionComponent::getx(){
	return m_X;
}
int& PositionComponent::gety(){
	return m_Y;
}

int PositionComponent::getPos() const {
	return m_X, m_Y;
}

float PositionComponent::getMaxVel() const{
	return MAX_VEL;
}

void PositionComponent::apply_natural_deceleration() {
	// check if abs of velocity x is less than NATURAL_DECELERATION
	// if it is set velocity x to zero if not do nothing
	if (abs(m_velocity_vector->x) < NATURAL_DECELERATION) {
		m_velocity_vector->x = 0;
	}
	else {
		// check if velocity x is positive
		if (m_velocity_vector->x > 0) {
			// if velocity x is positive subtract else add
			m_velocity_vector->x -= NATURAL_DECELERATION;
		}
		else {
			m_velocity_vector->x += NATURAL_DECELERATION;
		}
	}

	if (abs(m_velocity_vector->y) < NATURAL_DECELERATION) {
		m_velocity_vector->y = 0;
	}
	else {
		// repeat for y velocity
		if (m_velocity_vector->y > 0) {
			// if velocity y is positive subtract else add
			m_velocity_vector->y -= NATURAL_DECELERATION;
		}
		else {
			m_velocity_vector->y += NATURAL_DECELERATION;
		}
	}
}

void PositionComponent::update_current_acceleration() {
	if (m_Directions[DIR_LEFT]) {
		m_acceleration_vector->x = -m_speed;
	} 
	else if (m_Directions[DIR_RIGHT]) {
		m_acceleration_vector->x = m_speed;
	}
	if (m_Directions[DIR_DOWN]) {
		m_acceleration_vector->y = m_speed;
	}
	else if (m_Directions[DIR_UP]) {
		m_acceleration_vector->y = -m_speed;
	}
}

void PositionComponent::cap_velocity() {
	if (m_velocity_vector->x > MAX_VEL) {
		m_velocity_vector->x = MAX_VEL;
	}
	else if (m_velocity_vector->x < -MAX_VEL) {
		m_velocity_vector->x = -MAX_VEL;
	}
	if (m_velocity_vector->y > MAX_VEL) {
		m_velocity_vector->y = MAX_VEL;
	} 
	else if (m_velocity_vector->y < -MAX_VEL) {
		m_velocity_vector->y = -MAX_VEL;
	}
}

void PositionComponent::update(){

	//set_deltaTime_speedFix();


	//NATURAL_DECELERATION = DEFAULT_NATURAL_DECELERATION * m_deltaTime_speedFix;
	//m_speed = DEFAULT_SPEED * m_deltaTime_speedFix;

	cap_velocity();

	update_current_acceleration();

	// update velocity by acceleration/decelaration
	m_velocity_vector = *m_velocity_vector + m_acceleration_vector;
	apply_natural_deceleration();

	normalize();

	// update position by updated velocity
	m_X += (int)m_velocity_vector->x;
	m_Y += (int)m_velocity_vector->y;
}

void PositionComponent::draw(){

}

void PositionComponent::set_deltaTime_speedFix() {
	/*
	if (local != 0) {
		m_deltaTime_speedFix =  (((SDL_GetTicks() - unStartElapsedTime)*0.0001));
		log(nElapsedTime);
	}
	*/
}

void PositionComponent::normalize() {
	// normalize EZ
	// nevermind, not worth it
	
}
float PositionComponent::getVelMagnitude(){
	return m_velocity_vector->getMagnitude();
}

void PositionComponent::setVel(const int x, const int y) {
	m_velocity_vector->set((float)x, (float)y);
}

void PositionComponent::setAcc(const float& xAcc, const float& yAcc) {
	m_acceleration_vector->set(xAcc, yAcc);
}

void PositionComponent::setDirX(const bool& left, const bool& right) {
	m_Directions[DIR_LEFT] = left;
	m_Directions[DIR_RIGHT] = right;
}

void PositionComponent::setDirY(const bool& up, const bool& down) {
	m_Directions[DIR_DOWN] = down;
	m_Directions[DIR_UP] = up;
}

void PositionComponent::setDirX_left(const bool& dir) {
	m_Directions[DIR_LEFT] = dir;
}

void PositionComponent::setDirX_right(const bool& dir) {
	m_Directions[DIR_RIGHT] = dir;
}

void PositionComponent::setDirY_up(const bool& dir) {
	m_Directions[DIR_UP] = dir;
}

void PositionComponent::setDirY_down(const bool& dir) {
	m_Directions[DIR_DOWN] = dir;
}

std::array<bool, 4>& PositionComponent::getDir() {
	return m_Directions;
}

bool PositionComponent::isMoving() {
	if (m_Directions[DIR_LEFT] || m_Directions[DIR_RIGHT] || m_Directions[DIR_UP] || m_Directions[DIR_DOWN]) {
		return true;
	}
	return false;
}

bool PositionComponent::isMovingX() {
	if (m_Directions[DIR_LEFT] || m_Directions[DIR_RIGHT]) {
		return true;
	}
	return false;
}

bool PositionComponent::isMovingY() {
	if (m_Directions[DIR_UP] || m_Directions[DIR_DOWN]) {
		return true;
	}
	return false;
}