#include "WorldFileService.h"
#include "Model.h"
#include <stdlib.h>

World* createWorld(int worldIndex) {
	World* world = (World*)malloc(sizeof(World));
	world->catXPos = 0;
	world->catYPos = 0;
	world->cheeseXPos = 0;
	world->cheeseYPos = 0;
	world->mouseXPos = 0;
	world->mouseYPos = 0;
	world->currAnimal = 0;
	world->currTurn = 0;
	world->gameBoard = NULL;
	world->gameconfig = NULL;
	world->gameStatus = 0;
	world->isGameOver = 0;
	world->isPaused = 0;
	world->totalTurns = 0;
	world->firstAnimal = CAT;
	return world;
}

