#pragma once
#include "ECS.h"
#include "components.h"
#include "includes.h"


class DrawingComponent : public Component{
	SDL_Rect* m_destRect, * m_srcRect;
	SDL_Renderer* m_ren;
	SpriteSheet* m_spriteSheet;
	PositionComponent* m_posComp;
	float m_animationTime;
	float m_animationTick;
	float m_animationTickIncrement;
	unsigned int m_FPS;
	float m_framesPerState;
	unsigned int m_currentFrame;
	unsigned int m_prevDir;
	float m_maxFrameForAnimation;
	int* m_cameraOffsetX, * m_cameraOffsetY;

public:
	DrawingComponent(SDL_Renderer* inputRen, const std::string spriteTitle, int spriteSheetColumns, int spriteSheetRows, const unsigned int fps, int* cameraOffsetX, int* cameraOffsetY);

	~DrawingComponent();

	void update() override;

	void draw() override;

	void init() override;

private:
	void draw_frame_according_to_direction();

};

