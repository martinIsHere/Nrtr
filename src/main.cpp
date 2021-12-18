#include "GameEngine.h"
#include "ECS.h"
#include "Components.h"

// f12 moves into defenition
// f9 breakpoint

int main(int argc, char* argv[]) {

	GameEngine* a = new GameEngine(1280,720,"Tilebased", 60);
	while (a->alive()) {
		a->update();
	}
	

	return(0);

}




//14
//https://10fastfingers.com/share-badge/1_CI nice 