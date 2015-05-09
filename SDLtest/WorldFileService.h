#ifndef WorldFileService_h__
#define WorldFileService_h__

#include "Model.h"

#define MAX_WORLD_FILE_LENGTH 20
#define WORLD_FILE_PATH "worlds/world_"
#define WORLD_FILE_SUFFIX ".txt"
#define CAT_FIRST_PLAYER "cat"
#define MOUSE_FIRST_PLAYER "mouse"

#define EMPTY_SQUARE '#'
#define WALL_SQUARE 'W'
#define CAT_SQUARE 'C'
#define MOUSE_SQUARE 'M'
#define CHEESE_SQUARE 'P'

typedef struct worldData {
	char** gameBoard;
	int totalTurns;
	Animal firstAnimal;
} WorldData;


World* createWorld(int worldIndex);
WorldData* createEmptyWorldData();
void freeWorldData(WorldData *worldData);
void initializeBoard(char** board);
char** createBoard();
void freeBoard(char** board);
int createWorldData(int worldIndex, WorldData* worldData);
World* createEmptyWorld();
#endif // WorldFileService_h__