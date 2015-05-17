#include "MoveService.h"
#include "WorldFileService.h"
#include "EvaluationService.h"
#include "../ModelInfrastructure/Model.h"
#include "../main/ListUtils.h"
#include "../main/MiniMax.h"
#include <stdlib.h>
#include <stdio.h>

World* copyWorld(World* world) {
	World* newWorld = (World*)malloc(sizeof(World));
	if (newWorld == NULL) {
		perror("ERROR: malloc failed\n");
		exit(1);
	}
	newWorld->catXPos = world->catXPos;
	newWorld->catYPos = world->catYPos;  
	newWorld->mouseXPos = world->mouseXPos;
	newWorld->mouseYPos = world->mouseYPos;
	newWorld->cheeseXPos = world->cheeseXPos;
	newWorld->cheeseYPos = world->cheeseYPos;
	newWorld->gameBoard = copyBoard(world->gameBoard);
	newWorld->currAnimal = world->currAnimal;
	newWorld->totalTurns = world->totalTurns;
	newWorld->currTurn = world->currTurn;
	newWorld->gameStatus = world->gameStatus;
	newWorld->firstAnimal = world->firstAnimal;
	newWorld->isGameOver = world->isGameOver;
	newWorld->isPaused = world->isPaused;
	newWorld->gameValidity = world->gameValidity;
	return newWorld;
}

ListRef getChildWorlds(void* voidWorld) {

	World* world = (World*)voidWorld;

	ListRef children = newList(NULL);

	if (world->gameStatus != NO_WINNER) { // we don't want to explore subworlds of finished games
		return children;
	}

	Direction direction;
	for (direction = 0; direction < NUM_DIRECTIONS; direction++) {
		if (isMoveLegal(direction, world)) {
			World* childWorld = copyWorld(world);
			makeMove(direction, childWorld);
			
			append(children, childWorld);
			
		}

	}
	return children;
}

Direction getBestMove(World* world, int catSkill, int mouseSkill) {
	struct MiniMaxResult bestChild;
	int bestIndex;
	Direction bestDirection; // there is a difference between the index returned by getBestChild and the actual direction
	Direction currDirection;
	int count = 0;
	int skill;
	int isMaxPlayer;
	if (world->currAnimal == CAT) {
		skill = catSkill;
		isMaxPlayer = 0;
	}
	else {
		skill = mouseSkill;
		isMaxPlayer = 1;
	}

	bestChild = getBestChild(world, skill,  getChildWorlds,  freeVoidWorld,  evaluateBoard, isMaxPlayer);
	bestIndex = bestChild.index;

	for (currDirection = 0; currDirection < NUM_DIRECTIONS; currDirection++) {
		if (isMoveLegal(currDirection, world)) {
			if (count == bestIndex) {
				bestDirection = currDirection;
			}
			count++;
		}
	}

	return bestDirection;
}



void updateGameStatus(World* world) {
	if (world->currTurn == world->totalTurns) { //draw maxed turns
		world->gameStatus = DRAW;
		world->isGameOver = 1;
	}
	else if (isClose(world->mouseXPos, world->mouseYPos, world->catXPos, world->catYPos) == 1) {
		world->gameStatus = CAT_WON;
		world->isGameOver = 1;
	}
	else if (isClose(world->mouseXPos, world->mouseYPos, world->cheeseXPos, world->cheeseYPos) == 1) {
		world->gameStatus = MOUSE_WON;
		world->isGameOver = 1;
	}
	else {
		world->gameStatus = NO_WINNER;
		world->isGameOver = 0;
	}
}

Direction getDirectionFromPos(int xPos, int yPos, Model* model) {
	Animal playerAnimal = model->world->currAnimal;
	int playerXPos, playerYPos;
	if (playerAnimal == CAT) {
		playerXPos = model->world->catXPos;
		playerYPos = model->world->catYPos;
	}
	else if (playerAnimal == MOUSE) {
		playerXPos = model->world->mouseXPos;
		playerYPos = model->world->mouseYPos;
	}

	if ((playerXPos + 1 == xPos) && playerYPos == yPos) {
		return RIGHT;
	}
	else if ((playerXPos - 1 == xPos) && playerYPos == yPos) {
		return LEFT;
	}
	else if ((playerXPos == xPos) && playerYPos - 1 == yPos) {
		return UP;
	}
	else if ((playerXPos == xPos) && playerYPos  + 1 == yPos) {
		return DOWN;
	}
	else {
		return INVALID;
	}
}

int isMoveLegal(Direction direction, World* world) {
	Animal playerAnimal = world->currAnimal;
	int playerXPos, playerYPos;
	int newXPos, newYPos;

	if (direction == INVALID) {
		return 0;
	}

	if (playerAnimal == CAT) {
		playerXPos = world->catXPos;
		playerYPos = world->catYPos;
	}
	else if (playerAnimal == MOUSE) {
		playerXPos = world->mouseXPos;
		playerYPos = world->mouseYPos;
	}
	newXPos = playerXPos;
	newYPos = playerYPos;

	if (direction == UP) {
		newYPos--;
	}
	else if (direction == DOWN) {
		newYPos++;
	}
	else if (direction == RIGHT) {
		newXPos++;
	}
	else if (direction == LEFT) {
		newXPos--;
	}

	// out of bounds
	if (newXPos < 0 || newXPos > BOARD_SIZE - 1 || newYPos < 0 || newYPos > BOARD_SIZE - 1) {
		return 0;
	}

	// square is wall
	else if (world->gameBoard[newYPos][newXPos] != EMPTY_SQUARE) {
		return 0;
	}

	// square has cheese
	else if ((newXPos == world->cheeseXPos) && (newYPos == world->cheeseYPos)) {
		return 0;
	}

	return 1;
}

void makeMove(Direction direction, World* world) {
	Animal playerAnimal = world->currAnimal;
	int playerXPos, playerYPos;

	if (playerAnimal == CAT) {
		playerXPos = world->catXPos;
		playerYPos = world->catYPos;
	}
	else if (playerAnimal == MOUSE) {
		playerXPos = world->mouseXPos;
		playerYPos = world->mouseYPos;
	}

	if (direction == UP) {
		playerYPos--;
	}
	else if (direction == DOWN) {
		playerYPos++;
	}
	else if (direction == RIGHT) {
		playerXPos++;
	}
	else if (direction == LEFT) {
		playerXPos--;
	}

	if (playerAnimal == CAT) {
		world->catXPos = playerXPos;
		world->catYPos = playerYPos;
		world->currAnimal = MOUSE;
	}
	else if (playerAnimal == MOUSE) {
		world->mouseXPos = playerXPos;
		world->mouseYPos = playerYPos;
		world->currAnimal = CAT;
	}

	world->currTurn++;
	updateGameStatus(world);


	
}

