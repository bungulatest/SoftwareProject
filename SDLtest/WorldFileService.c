#include "WorldFileService.h"
#include "Model.h"
#include "GameWindow.h"
#include <stdlib.h>



World* createEmptyWorld() {
	World* world = (World*)malloc(sizeof(World));
	world->catXPos = -1;
	world->catYPos = -1;
	world->cheeseXPos = -1;
	world->cheeseYPos = -1;
	world->mouseXPos = -1;
	world->mouseYPos = -1;
	world->currAnimal = 0;
	world->currTurn = 0;
	world->gameBoard = createBoard();
	world->gameconfig = NULL;
	world->gameStatus = NO_WINNER;
	world->isGameOver = 0;
	world->isPaused = 0;
	world->totalTurns = 0;
	world->firstAnimal = 0;
	world->gameValidity = 0;
	return world;
}

World* createWorld(int worldIndex) {
	int i, j;
	char currSquare;
	World* world = createEmptyWorld();
	WorldData* worldData = createEmptyWorldData();
	createWorldDataFromFile(worldIndex, worldData);
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
					world->catYPos = i;
					world->catXPos = j;
				}
				else if (currSquare == MOUSE_SQUARE) {
					world->mouseYPos = i;
					world->mouseXPos = j;
				}
				else if (currSquare == CHEESE_SQUARE) {
					world->cheeseYPos = i;
					world->cheeseXPos = j;
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

void freeWorld(World* world) {
	if (world->gameBoard != NULL) {
		freeBoard(world->gameBoard);
	}
	free(world);
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
	char** board = (char**)malloc(sizeof(char*) * BOARD_SIZE);
	if (board == NULL) {
		//TODO: handle error
		perror("ERROR: malloc failed");
		return NULL;
	}

	for (i = 0; i < BOARD_SIZE; ++i) {
		board[i] = (char*)malloc(sizeof(char) * BOARD_SIZE);
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

char** copyBoard(char** board) {
	char** newBoard = createBoard();
	if (newBoard == NULL) {
		return NULL; // malloc failes on createBoard()
	}
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			newBoard[i][j] = board[i][j];
		}
	}
	return newBoard;
}

void freeBoard(char** board) {
	int i;
	for (i = 0; i < BOARD_SIZE; ++i) {
		free(board[i]);
	}
	free(board);
}

int createWorldDataFromFile(int worldIndex, WorldData* worldData) {
	FILE* file = NULL;
	char filePath[MAX_WORLD_FILE_LENGTH];
	int i, j;
	char currSquare;

	sprintf(filePath, "%s%d%s", WORLD_FILE_PATH, worldIndex, WORLD_FILE_SUFFIX);
	
	file = fopen(filePath, "r");
	if (file == NULL) {
		perror(ERROR_OPEN_FILE);
		return 1;
	}

	int totalTurns = 0;
	char firstAnimalString[6] = { 0 };

	if (fscanf(file, "%d", &totalTurns) < 1) {
		perror(ERROR_READ_FILE);
		return 1;
	}
	if (fscanf(file, "%s", firstAnimalString) < 1) {
		perror(ERROR_READ_FILE);
		return 1;
	}

	fgetc(file);
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			currSquare = fgetc(file);
			worldData->gameBoard[i][j] = currSquare;
		}
		fgetc(file);
	}

	if (fclose(file)) {
		perror(ERROR_CLOSE_FILE);
		return 1;
	}

	worldData->totalTurns = totalTurns;
	if (!strcmp(firstAnimalString, CAT_FIRST_PLAYER)) {
		worldData->firstAnimal = CAT;
	}
	else if (!strcmp(firstAnimalString, MOUSE_FIRST_PLAYER)) {
		worldData->firstAnimal = MOUSE;
	}

	return 0;
}


GameValidity isWorldValid(World* world) {
	if (world->catXPos == -1) {
		return CAT_IS_MISSING;
	}
	if (world->mouseXPos == -1) {
		return MOUSE_IS_MISSING;
	}
	if (world->cheeseXPos == -1) {
		return CHEESE_IS_MISSING;
	}
	else {
		return VALID;
	}
}

WorldData* convertWorldToWorldData(World* world) {
	WorldData* worldData = createEmptyWorldData();
	//if (worldData == NULL){ TODO: 
	int i, j;

	worldData->firstAnimal = world->firstAnimal;
	worldData->totalTurns = world->totalTurns;

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			worldData->gameBoard[i][j] = world->gameBoard[i][j];
		}
	}

	worldData->gameBoard[world->catYPos][world->catXPos] = CAT_SQUARE;
	worldData->gameBoard[world->mouseYPos][world->mouseXPos] = MOUSE_SQUARE;
	worldData->gameBoard[world->cheeseYPos][world->cheeseXPos] = CHEESE_SQUARE;

	return worldData;
}

void saveWorldDataToFile(WorldData* worldData, int worldIndex) {
	FILE* file = NULL;
	int i, j;
	
	char filePath[MAX_WORLD_FILE_LENGTH];
	sprintf(filePath, "%s%d%s", WORLD_FILE_PATH, worldIndex, WORLD_FILE_SUFFIX);
	file = fopen(filePath, "w");
	if (file == NULL) {
		perror(ERROR_OPEN_FILE);
		return ;
	}
	if (fprintf(file, "%d\n", DEFAULT_NUM_OF_TURNS) < 0) {
		perror(ERROR_WRITE_FILE);
		return ;
	}
	if (worldData->firstAnimal == CAT) {
		if (fprintf(file, "%s\n", CAT_FIRST_PLAYER) < 0) {
			perror(ERROR_WRITE_FILE);
			return ;
		}
	}
	else {
		if (fprintf(file, "%s\n", MOUSE_FIRST_PLAYER) < 0) {
			perror(ERROR_WRITE_FILE);
			return ;
		}
	}

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (fprintf(file, "%c", worldData->gameBoard[i][j]) < 0) {
				perror(ERROR_WRITE_FILE);
				return ;
			}
		}
		if (fprintf(file, "\n") < 0) {
			perror(ERROR_WRITE_FILE);
			return ;
		}
	}

	if (fclose(file)) {
		perror(ERROR_CLOSE_FILE);
		return ;
	}
	return ;
}

void saveWorldToFile(int worldIndex, World* world) {
	WorldData* worldData = convertWorldToWorldData(world);
	saveWorldDataToFile(worldData, worldIndex);

	freeWorldData(worldData);

	return ;
}


