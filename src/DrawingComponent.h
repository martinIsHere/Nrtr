#pragma once
#include "ECS.h"
#include "components.h"
#include "includes.h"
#include "Camera.h"

class DrawingComponent : public Component{
	SDL_Rect* m_destRect, * m_srcRect;
	SDL_Renderer* m_ren;
	SpriteSheet* m_spriteSheet;
	PositionComponent* m_posComp;
	float m_animationTime;
	int m_animationTick;
	unsigned int m_FPS;
	float m_animationFramesPerState;
	int m_framesPerImage;
	unsigned int m_frameCounter;
	unsigned int m_prevDir;
	const int* m_cameraOffsetX, * m_cameraOffsetY;
	const Camera* m_cam;

public:
	DrawingComponent(
		SDL_Renderer* inputRen, // for drawing
		const std::string spriteTitle, // path of texture sheet
		const int walkSpriteWidthNHeight,
		const int spriteSheetColumns, 
		const int spriteSheetRows, 
		const unsigned int fps, 
		const Camera* cam
		//int* cameraOffsetX, // from camera class
		//int* cameraOffsetY
		);

	~DrawingComponent();

	void update() override;

	void draw() override;

	void init() override;

private:
	void draw_frame_according_to_direction();

	void updateWithoutDrawing_frame_according_to_direction(); // quick fix

	const bool is_in_viewable_area()const;

};

