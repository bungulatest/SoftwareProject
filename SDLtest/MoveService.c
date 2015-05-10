#include "MoveService.h"
#include "WorldFileService.h"
#include "GameWindow.h"

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

int isMoveLegal(Direction direction, Model* model) {
	Animal playerAnimal = model->world->currAnimal;
	int playerXPos, playerYPos;
	int newXPos, newYPos;

	if (direction == INVALID) {
		return 0;
	}

	if (playerAnimal == CAT) {
		playerXPos = model->world->catXPos;
		playerYPos = model->world->catYPos;
	}
	else if (playerAnimal == MOUSE) {
		playerXPos = model->world->mouseXPos;
		playerYPos = model->world->mouseYPos;
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
	if (model->world->gameBoard[newYPos][newXPos] != EMPTY_SQUARE) {
		return 0;
	}

	// square has cheese
	if ((newXPos == model->world->cheeseXPos) && (newYPos == model->world->cheeseYPos)) {
		return 0;
	}

	return 1;
}

void makeMove(Direction direction, Model* model) {
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
		model->world->catXPos = playerXPos;
		model->world->catYPos = playerYPos;
		model->world->currAnimal = MOUSE;
	}
	else if (playerAnimal == MOUSE) {
		model->world->mouseXPos = playerXPos;
		model->world->mouseYPos = playerYPos;
		model->world->currAnimal = CAT;
	}

	model->world->currTurn++;
	if (model->world->currTurn == model->world->totalTurns) {
		model->world->gameStatus = DRAW;
		model->world->isGameOver = 1;
	}
	
}

