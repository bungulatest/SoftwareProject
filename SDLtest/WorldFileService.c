#include "WorldFileService.h"
#include "Model.h"
#include "GameWindow.h"
#include <stdlib.h>

World* createEmptyWorld() {
	World* world = (World*)malloc(sizeof(World));
	world->catXPos = 0;
	world->catYPos = 0;
	world->cheeseXPos = 0;
	world->cheeseYPos = 0;
	world->mouseXPos = 0;
	world->mouseYPos = 0;
	world->currAnimal = 0;
	world->currTurn = 0;
	world->gameBoard = createBoard();
	world->gameconfig = NULL;
	world->gameStatus = NO_WINNER;
	world->isGameOver = 0;
	world->isPaused = 0;
	world->totalTurns = 0;
	world->firstAnimal = 0;
	return world;
}

World* createWorld(int worldIndex) {
	int i, j;
	char currSquare;
	World* world = createEmptyWorld();
	WorldData* worldData = createEmptyWorldData();
	createWorldData(worldIndex, worldData);
	world->firstAnimal = worldData->firstAnimal;
	world->currAnimal = worldData->firstAnimal;
	world->totalTurns = worldData->totalTurns;
	
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			currSquare = worldData->gameBoard[i][j];
			if (currSquare == WALL_SQUARE) {
				world->gameBoard[i][j] = WALL_SQUARE;
			}
			else {
				world->gameBoard[i][j] = EMPTY_SQUARE;

				if (currSquare == CAT_SQUARE) {
					world->catXPos = i;
					world->catYPos = j;
				}
				else if (currSquare == MOUSE_SQUARE) {
					world->mouseXPos = i;
					world->mouseYPos = j;
				}
				else if (currSquare == CHEESE_SQUARE) {
					world->cheeseXPos = i;
					world->cheeseYPos = j;
				}
			}
		}
	}

	return world;

}

WorldData* createEmptyWorldData() {
	WorldData* worldData = (WorldData*)(malloc)(sizeof(WorldData));
	if (worldData == NULL) {
		//TODO: handle error
		perror("ERROR: malloc failed");
		return NULL;
	}

	worldData->gameBoard = createBoard();
	if (worldData->gameBoard == NULL) {
		//TODO: handle error
		perror("ERROR: malloc failed");
		free(worldData);
		return NULL;
	}

	return worldData;
}

void freeWorldData(WorldData *worldData) {
	if (worldData->gameBoard != NULL) {
		freeBoard(worldData->gameBoard);
	}
	free(worldData);
}

void initializeBoard(char** board) {
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = EMPTY_SQUARE;

		}
	}

}

char** createBoard() {
	int i, j;
	char** board = (char**)calloc(sizeof(char*), BOARD_SIZE);
	if (board == NULL) {
		//TODO: handle error
		perror("ERROR: malloc failed");
		return NULL;
	}

	for (i = 0; i < BOARD_SIZE; ++i) {
		board[i] = (char*)calloc(sizeof(char), BOARD_SIZE);
		if (board[i] == NULL) {
			//TODO: handle error
			perror("ERROR: malloc failed");
			return NULL;
			for (j = 0; j < i; j++) {
				free(board[j]);
			}
			free(board);
			return NULL;
		}
	}

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = EMPTY_SQUARE;

		}
	}
	return board;
}

void freeBoard(char** board) {
	int i;
	for (i = 0; i < BOARD_SIZE; ++i) {
		free(board[i]);
	}
	free(board);
}

int createWorldData(int worldIndex, WorldData* worldData) {
	FILE* file = NULL;
	char filePath[MAX_WORLD_FILE_LENGTH];
	int i, j;
	char currSquare;

	sprintf(filePath, "%s%d%s", WORLD_FILE_PATH, worldIndex, WORLD_FILE_SUFFIX);
	
	file = fopen(filePath, "r");
	if (file == NULL) {
		perror("ERROR: could not open world file");
		return 1;
	}

	int totalTurns = 0;
	char firstAnimalString[6];

	if (fscanf(file, "%d", &totalTurns)) {
		perror("ERROR: could not read world file");
		return 1;
	}
	if (fscanf(file, "%s", firstAnimalString)) {
		perror("ERROR: could not read world file");
		return 1;
	}

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			currSquare = fgetc(file);
			worldData->gameBoard[i][j] = currSquare;
		}
		fgetc(file);
	}

	if (fclose(file)) {
		perror("ERROR: could not close world file");
		return 1;
	}

	worldData->totalTurns = totalTurns;
	if (strcmp(firstAnimalString, CAT_FIRST_PLAYER)) {
		worldData->firstAnimal = CAT;
	}
	else if (strcmp(firstAnimalString, MOUSE_FIRST_PLAYER)) {
		worldData->firstAnimal = MOUSE;
	}

	return 0;
}