#include "PositionComponent.h"


PositionComponent::PositionComponent(int x, int y) : m_X(x), m_Y(y) {
	m_velocity_vector = nullptr;
	m_acceleration_vector = nullptr;
	m_Magnitude = 0;
	m_Directions = { false, false, false, false };
	m_facingDir = 0;
	default_acceleration = float(1.6 * (double)TILE_SIZE_PIXELS / (double)targetFPS); // DEFAULT ACCELERATION   // 1.4 if TILE_SIZE_PIXELS = 64
	default_natural_deceleration = float(0.5 * (double)TILE_SIZE_PIXELS / (double)targetFPS); // 0.8 if TILE_SIZE_PIXELS = 64 and targetFPS = 60
	natural_deceleration = default_natural_deceleration;
	max_vel = float(8 * (double)TILE_SIZE_PIXELS / (double)targetFPS); // 10 if TILE_SIZE_PIXELS = 64 and targetFPS = 60
	m_deltaTime_speedFix = 0;
	isAbleToMove = true;
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

float PositionComponent::getMaxVel() const{
	return max_vel;
}

void PositionComponent::apply_natural_deceleration() {
	// check if abs of velocity x is less than NATURAL_DECELERATION
	// if it is set velocity x to zero if not do nothing
	if (abs(m_velocity_vector->x) < natural_deceleration) {
		m_velocity_vector->x = 0;
	}
	else {
		// check if velocity x is positive
		if (m_velocity_vector->x > 0) {
			// if velocity x is positive subtract else add
			m_velocity_vector->x -= natural_deceleration;
		}
		else {
			m_velocity_vector->x += natural_deceleration;
		}
	}

	if (abs(m_velocity_vector->y) < natural_deceleration) {
		m_velocity_vector->y = 0;
	}
	else {
		// repeat for y velocity
		if (m_velocity_vector->y > 0) {
			// if velocity y is positive subtract else add
			m_velocity_vector->y -= natural_deceleration;
		}
		else {
			m_velocity_vector->y += natural_deceleration;
		}
	}
}

void PositionComponent::update_current_acceleration() {
	if (m_Directions[DIR_LEFT]) {
		m_acceleration_vector->x = -default_acceleration;
	} 
	else if (m_Directions[DIR_RIGHT]) {
		m_acceleration_vector->x = default_acceleration;
	}
	if (m_Directions[DIR_DOWN]) {
		m_acceleration_vector->y = default_acceleration;
	}
	else if (m_Directions[DIR_UP]) {
		m_acceleration_vector->y = -default_acceleration;
	}
	if ((m_Directions[DIR_LEFT] || m_Directions[DIR_RIGHT]) 
		&& (m_Directions[DIR_UP] || m_Directions[DIR_DOWN])) {
		m_acceleration_vector->x *= 0.9f;
		m_acceleration_vector->y *= 0.9f;
	}
}

void PositionComponent::cap_velocity() {
	if (m_velocity_vector->x > max_vel) {
		m_velocity_vector->x = max_vel;
	}
	else if (m_velocity_vector->x < -max_vel) {
		m_velocity_vector->x = -max_vel;
	}
	if (m_velocity_vector->y > max_vel) {
		m_velocity_vector->y = max_vel;
	} 
	else if (m_velocity_vector->y < -max_vel) {
		m_velocity_vector->y = -max_vel;
	}
}

void PositionComponent::update(){

	//set_deltaTime_speedFix();


	//NATURAL_DECELERATION = default_natural_deceleration * m_deltaTime_speedFix;
	//default_acceleration = DEFAULT_SPEED * m_deltaTime_speedFix;
	if (isAbleToMove) {
		m_acceleration_vector->x = 0;
		m_acceleration_vector->y = 0;
		updateFacingDir();


		update_current_acceleration();

		// update velocity by acceleration/decelaration
		m_velocity_vector = *m_velocity_vector + m_acceleration_vector;
		apply_natural_deceleration();

		cap_velocity();

		// update position by updated velocity
		m_X += (int)m_velocity_vector->x;
		m_Y += (int)m_velocity_vector->y;
	}
	else {
		setAcc(0, 0);
		setVel(0, 0);
	}
}

// rendered pointless
void PositionComponent::updateFacingDir() {
	
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

void PositionComponent::setVel(const float x, const float y) {
	m_velocity_vector->set((float)x, (float)y);
}

void PositionComponent::setPos(const int x, const int y) {
	m_X = x;
	m_Y = y;
}

void PositionComponent::moveForward(int d) {
	if (m_facingDir == DIR_LEFT) m_X -= d;
	else if (m_facingDir == DIR_RIGHT) m_X += d;
	else if (m_facingDir == DIR_DOWN) m_Y += d;
	else m_Y -= d; 
}

void PositionComponent::setAcc(const float& xAcc, const float& yAcc) {
	m_acceleration_vector->set(xAcc, yAcc);
}

void PositionComponent::setDirX(const bool& left, const bool& right) {
	m_Directions[DIR_LEFT] = left;
	m_Directions[DIR_RIGHT] = right;
	if (left) {
		m_facingDir = DIR_LEFT;
	}
	else if(right) {
		m_facingDir = DIR_RIGHT;
	}
}

void PositionComponent::setDirY(const bool& up, const bool& down) {
	m_Directions[DIR_DOWN] = down;
	m_Directions[DIR_UP] = up;
	if (down) {
		m_facingDir = DIR_DOWN;
	}
	else if (up) {
		m_facingDir = DIR_UP;
	}
}

void PositionComponent::setDirX_left(const bool& dir) {
	m_Directions[DIR_LEFT] = dir;
	if (dir) {
		m_facingDir = DIR_LEFT;
	}
}

void PositionComponent::setDirX_right(const bool& dir) {
	m_Directions[DIR_RIGHT] = dir;
	if (dir) {
		m_facingDir = DIR_RIGHT;
	}
}

void PositionComponent::setDirY_up(const bool& dir) {
	m_Directions[DIR_UP] = dir;
	if (dir) {
		m_facingDir = DIR_UP;
	}
}

void PositionComponent::setDirY_down(const bool& dir) {
	m_Directions[DIR_DOWN] = dir;
	if (dir) {
		m_facingDir = DIR_DOWN;
	}
}

void PositionComponent::setDir(uint32_t dir, bool value) {
	m_Directions[dir] = value;
	if (value) {
		m_facingDir = dir;
	}
}

std::array<bool, 4>& PositionComponent::getDir() {
	return m_Directions;
}

uint32_t& PositionComponent::getFacingDir() {
	return m_facingDir;
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

float& PositionComponent::getVelx() {
	return m_velocity_vector->x;
}

float& PositionComponent::getVely() {
	return m_velocity_vector->y;
}

Vector* PositionComponent::getVel_vector() {
	return m_velocity_vector;
}

bool PositionComponent::isMovingLeftVel() {
	if (m_velocity_vector->x < 0) {
		return true;
	}
	return false;
}

bool PositionComponent::isMovingRightVel() {
	if (m_velocity_vector->x > 0) {
		return true;
	}
	return false;
}

bool PositionComponent::isMovingUpVel() {
	if (m_velocity_vector->y < 0) {
		return true;
	}
	return false;
}

bool PositionComponent::isMovingDownVel() {
	if (m_velocity_vector->y > 0) {
		return true;
	}
	return false;
}

bool PositionComponent::isMovingVel() {
	if (m_velocity_vector->x != 0 && m_velocity_vector != 0) {
		return true;
	}
	return false;
}

bool PositionComponent::isMovingVelX() {
	if (m_velocity_vector->x != 0) {
		return true;
	}
	return false;
}

bool PositionComponent::isMovingVelY() {
	if (m_velocity_vector->y != 0) {
		return true;
	}
	return false;
}

void PositionComponent::set_isAbleToMove(bool a) {
	isAbleToMove = a;
}