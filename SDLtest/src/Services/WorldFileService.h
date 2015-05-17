#ifndef WorldFileService_h__
#define WorldFileService_h__

#include "../ModelInfrastructure/Model.h"
#include <stdio.h>

#define MAX_WORLD_FILE_LENGTH 20
#define WORLD_FILE_PATH "worlds/world_"
#define WORLD_FILE_SUFFIX ".txt"
#define CAT_FIRST_PLAYER "cat"
#define MOUSE_FIRST_PLAYER "mouse"

#define ERROR_WRITE_FILE "ERROR: could not write to world file"
#define ERROR_OPEN_FILE "ERROR: could not open world file"
#define ERROR_CLOSE_FILE "ERROR: could not close world file"
#define ERROR_READ_FILE "ERROR: could not read from world file"

#define EMPTY_SQUARE '#'
#define WALL_SQUARE 'W'
#define CAT_SQUARE 'C'
#define MOUSE_SQUARE 'M'
#define CHEESE_SQUARE 'P'

#define DEFAULT_NUM_OF_TURNS 20

typedef struct worldData {
	char** gameBoard;
	int totalTurns;
	Animal firstAnimal;
} WorldData;


World* createWorld(int worldIndex);
WorldData* createEmptyWorldData();
void freeWorldData(WorldData *worldData);
void freeWorld(World* world);
void freeVoidWorld(void* voidWorld);
void initializeBoard(char** board);
char** createBoard();
char** copyBoard(char** board);
void freeBoard(char** board);
int createWorldDataFromFile(FILE* file, WorldData* worldData);
int createWorldDataFromIndex(int worldIndex, WorldData* worldData);
GameValidity isWorldValid(World* world);
WorldData* convertWorldToWorldData(World* world);
void saveWorldDataToFile(WorldData* worldData, int worldIndex);
void saveWorldToFile(int worldIndex, World* world);
World* createEmptyWorld();
void convertWorldDataToWorld(World* world, WorldData* worldData);
#endif // WorldFileService_h__