#include "Scene.h"

Manager* Scene::entityManagerPtr = nullptr;
std::array<GameMap*, 10>* Scene::mapArrayPtr = nullptr;
size_t* Scene::currentMapIdPtr = nullptr;
std::vector<Entity*>* Scene::arrayOfActiveEntitiesPtr = nullptr;
SDL_Renderer* Scene::renPtr = nullptr;
uint32_t* Scene::nWinWidthPtr = nullptr;
uint32_t* Scene::nWinHeightPtr = nullptr;
Entity* Scene::playerEntity = nullptr;