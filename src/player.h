#pragma once
#include "includes.h"
#include "GameMap.h"

class Player {
public:
	Player(SDL_Renderer* ren1, GameMap* map1, int nSize1 = 64, SDL_Texture* tex1 = nullptr, int nBlockSize = 0, int nCols = 0, int nRows = 0);
	
	//Player(SDL_Renderer* ren1, GameMap* map1, int nSize1 = 64, const char* fileName = nullptr, int nBlockSize = 0, int nCols = 0, int nRows = 0);

	~Player();
	
	void move();

	void draw();

	void setSprite(SDL_Texture* tex1);

	void setSprite(const char* fileName);

	void setVel(float x, float y);

	float* getVel();

	void setPos(int x, int y);

	int* getPos();

	SpriteSheet* getSpriteSheet();

	void setSpriteSheet(SpriteSheet* sh);

	void setMaxSpeed(int nMax);

	void setSize(int size);
	
	int getSize();

	void update();

private:

	void setup();

	void updateCurrentSprite();

	float fPlayerVel[2];
	int nPlayerPos[2], nSize, nCurrentSprite, nMaxSpeed;
	SDL_Renderer* ren;
	SpriteSheet* spriteSheet0;
	SDL_Rect* srcRect, * dstRect;
	GameMap* map;
};