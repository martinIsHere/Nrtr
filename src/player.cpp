#include "Player.h"

void Player::setup() {
	nMaxSpeed = 10;
	srcRect = new SDL_Rect{ 0, 0, 0, 0 }, dstRect = new SDL_Rect{ 0, 0, 0, 0 }, nCurrentSprite = 0;
	nPlayerPos[0] = 0, nPlayerPos[1] = 0;
}

Player::Player(SDL_Renderer* ren1, GameMap* map1, int nSize1, SDL_Texture* tex1, int nBlockSize, int nCols, int nRows) {
	nSize = nSize1;
	ren = ren1;
	map = map1;

	spriteSheet0 = new SpriteSheet{tex1, nBlockSize, nCols, nRows};
	if (spriteSheet0->tex == nullptr) log("SpriteSheet0->tex == nullptr");

	setup();
}

Player::Player(SDL_Renderer* ren1, GameMap* map1, int nSize1, const char* sFileName, int nBlockSize, int nCols, int nRows) {
	nSize = nSize1;
	ren = ren1;
	map = map1;
	spriteSheet0 = new SpriteSheet{ SDL_CreateTextureFromSurface(ren, SDL_LoadBMP(sFileName)), nBlockSize, nCols, nRows };
	if (spriteSheet0->tex == nullptr) log("SpriteSheet0->tex == nullptr");

	setup();
}

Player::~Player() {  
	delete spriteSheet0, srcRect, dstRect;
	//useless
	delete[] &fPlayerVel, &nPlayerPos;
}

void Player::move() {
	nPlayerPos[0] += int(fPlayerVel[0]*map->getBlockSize());
	nPlayerPos[1] += int(fPlayerVel[1]*map->getBlockSize());
	fPlayerVel[0] = 0;
	fPlayerVel[1] = 0;
}



int Player::getSize() {
	return nSize;
}

void Player::setSize(int size) {
	nSize = size;
}

void Player::draw() {
	//source rect
	srcRect->x = nCurrentSprite % spriteSheet0->nWidth;
	srcRect->y = (int)(nCurrentSprite / spriteSheet0->nWidth);
	srcRect->w = spriteSheet0->nSize;
	srcRect->h = spriteSheet0->nSize;

	//destination rect
	dstRect->x = nPlayerPos[0];
	dstRect->y = nPlayerPos[1];
	dstRect->w = nSize;
	dstRect->h = nSize;



	SDL_RenderCopy(ren, spriteSheet0->tex, srcRect, dstRect);
}

void Player::update() {
	move();

	//fPlayerAcc[0] = fPlayerAcc[0] > 0 ? fPlayerAcc[0] - 0.3 : fPlayerAcc[0] < 0 ? fPlayerAcc[0] + 0.3 : fPlayerAcc[0];

	//fPlayerVel[0] = 

}

void Player::setSprite(SDL_Texture* tex1) {
	spriteSheet0->tex = tex1;
}

void Player::updateCurrentSprite() {
	
}

void Player::setSprite(const char* fileName) {
	spriteSheet0->tex = SDL_CreateTextureFromSurface(ren, SDL_LoadBMP(fileName));
}

void Player::setVel(float x, float y) {
	fPlayerVel[0] = x;
	fPlayerVel[1] = y;
}

float* Player::getVel() {
	return fPlayerVel;
}



void Player::setPos(int x, int y) {
	nPlayerPos[0] = x;
	nPlayerPos[1] = y;
}

int* Player::getPos() {
	return nPlayerPos;
}

void Player::setMaxSpeed(int nMax) {
	nMaxSpeed = nMax;
}

SpriteSheet* Player::getSpriteSheet() {
	return spriteSheet0;
}

void Player::setSpriteSheet(SpriteSheet* sh) {
	spriteSheet0 = sh;
}