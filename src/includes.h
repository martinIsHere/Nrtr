#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <thread>
#include "SDL.h"
#include "Vector.h"
#include <stdint.h>
#define PI 3.14159
#define log(a){std::cout << a << "\n";}
#define logh(a){std::cout << std:hex << a << "\n";}
#define assertN(v, t) { if (v == t){std::cout << "Error: "; exit(-1);} }
//#define assert(v, t) { if (v != t){std::cout << "Error: "; exit(-1);} }
#define assert(v){if(v==nullptr){std::cout << "!---nullptr---!\n";}}
#define DIR_LEFT 0
#define DIR_RIGHT 1
#define DIR_UP 2
#define DIR_DOWN 3
#define TILE_SIZE 64
class GameEngine;
class GameMap;
extern uint32_t unStartElapsedTime;
extern int nElapsedTime;
extern uint32_t actualFPS;

// unused
extern uint32_t WIN_WIDTH;
extern uint32_t WIN_HEIGHT;



struct SpriteSheet {
	SDL_Texture* tex;
	int nSize, nWidth, nHeight;
};