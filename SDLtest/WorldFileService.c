#include "WorldFileService.h"
#include "Model.h"
#include <stdlib.h>

World* createWorld(int worldIndex) {
	World* world = (World*)malloc(sizeof(World));
	world->firstAnimal = CAT;
	return world;
}

