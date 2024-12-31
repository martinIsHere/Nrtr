#include "TheaterEngine.h"
#include "SceneCollection.h"
#include "Scene.h"
#include "Components.h"

TheaterEngine::TheaterEngine() {
	gameStateManagerPtr = nullptr;
	currentScenePtr = nullptr;
	entityManagerPtr = nullptr;
	currentlyLoadedScenesPtr = new std::array<Scene*, MAX_LOADED_SCENES>();

}

TheaterEngine::~TheaterEngine() {

}

void TheaterEngine::init(
	GameStateManager* gameStateManagerPtr,
	Manager* entityManagerPtr,
	GameMap** currentMapPtrPtr,
	SDL_Renderer* renPtr,
	uint32_t* nWinWidthPtr,
	uint32_t* nWinHeightPtr
	) {
	this->entityManagerPtr = entityManagerPtr;
	this->gameStateManagerPtr = gameStateManagerPtr;
	Scene::entityManagerPtr = entityManagerPtr;
	Scene::currentMapPtrPtr = currentMapPtrPtr;
	Scene::arrayOfActiveEntitiesPtr = entityManagerPtr->getEntityArrayPointer();
	Scene::renPtr = renPtr;
	Scene::nWinWidthPtr = nWinWidthPtr;
	Scene::nWinHeightPtr = nWinHeightPtr;
	Scene::interactionCoords = new Point(NULL, NULL);
	Scene::playerHasInteracted = new bool(false);
	Scene::theaterEnginePtr = this;


	// TODO: relative to prev save init a scene
	changeCurrentScene<OpeningScene>();
	currentScenePtr->init();
}

void TheaterEngine::makeEntityMove_constantSpeedNoDiagonalMovement(
	Entity* ent,
	int xDest,
	int yDest,
	float speed,
	const int waitTimeFirstStop_inSeconds, 
	const int waitTimeSecondStop_inSeconds, 
	bool xMove_before_yMove
	) {
	int waitTimeFirstStop_inFrames = waitTimeFirstStop_inSeconds * targetFPS;
	int waitTimeSecondStop_inFrames = waitTimeSecondStop_inSeconds * targetFPS;

}
void TheaterEngine::teleportEntity(
	Entity* ent,
	int xDest,
	int yDest
	) {
	ent->getComponent<PositionComponent>().setPos(xDest, yDest);
}

bool TheaterEngine::changeCurrentMap(GameMap* in_currentMapPtr) {
	if (in_currentMapPtr != nullptr) {
		for (Entity* entity : *entityManagerPtr->getEntityArrayPointer()) {
			if (entity->hasComponent<CollisionComponent>()) {
				entity->getComponent<CollisionComponent>().loadNewMap(in_currentMapPtr);
			}
			if (entity->hasComponent<DrawingComponent>()) {
				entity->getComponent<DrawingComponent>().loadNewCamera(in_currentMapPtr->getCam());
			}
			if (entity->hasComponent<PositionComponent>()) {
				entity->getComponent<PositionComponent>().setPos(TILE_SIZE_PIXELS * 2, TILE_SIZE_PIXELS * 3);
			}
		}
		*Scene::currentMapPtrPtr = in_currentMapPtr;
		return true;
	}
	return false;
}

template <typename T>
Scene* TheaterEngine::createOrGetScene() const {
	if ((*currentlyLoadedScenesPtr)[Scene::getSceneTypeID<T>()] != nullptr) 
		return (*currentlyLoadedScenesPtr)[Scene::getSceneTypeID<T>()];
	return new T();
}

template <typename T>  // return true if the new scene has already been loaded
bool TheaterEngine::changeCurrentScene() {
	Scene* in_currentScenePtr = createOrGetScene<T>();
	if (!in_currentScenePtr) return false;
	currentScenePtr = in_currentScenePtr;
	for (Scene* scene : *currentlyLoadedScenesPtr) {
		if (scene = in_currentScenePtr) return true;
	}
	(*currentlyLoadedScenesPtr)[Scene::getSceneTypeID<T>()] = in_currentScenePtr;
	return true;
}


Entity* TheaterEngine::getPlayerEntity() {
	return Scene::playerEntity;
}

GameMap* TheaterEngine::getCurrentMap() {
	return *Scene::currentMapPtrPtr;
}

void TheaterEngine::update() {
	currentScenePtr->update();
}

void TheaterEngine::draw() {
	currentScenePtr->draw();
}