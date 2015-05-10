#include "EvaluationService.h"
#include "Model.h"
#include "WorldFileService.h"
#include "ListUtils.h"
#include "GameWindow.h"
#include <stdio.h>

void moveNode(int xPos, int yPos, char** tempBoard, Direction direction, Node* node) {

	if (direction == UP) {
		node->yPos--;
	}
	else if (direction == DOWN) {
		node->yPos++;
	}
	else if (direction == RIGHT) {
		node->xPos++;
	}
	else if (direction == LEFT) {
		node->xPos--;
	}
	node->distance++;

	// out of bounds
	if (node->xPos < 0 || node->xPos > BOARD_SIZE - 1 || node->yPos < 0 || node->yPos > BOARD_SIZE - 1) {
		free(node);
		node = NULL;
	}

	// square is wall
	if (tempBoard[node->yPos][node->xPos] != EMPTY_SQUARE) {
		free(node);
		node = NULL;
	}
}

int calcDistance(int xPos1, int yPos1, int xPos2, int yPos2, Model* model) {
	char** tempBoard = copyBoard(model->world->gameBoard);
	int distanceToDest = -1; // unreachable

	// set the cheese square as a wall in the temp board, because the cat can't move through cheese
	tempBoard[model->world->cheeseYPos][model->world->cheeseXPos] = WALL_SQUARE;

	Node* node = NULL;
	Node* node1 = (Node*)malloc(sizeof(Node));
	node1->xPos = xPos1;
	node1->yPos = yPos1;
	node1->distance = 0;
	int isFoundDest = 0;

	ListRef queue = newList(node1);
	ListRef queueHead = queue;

	while (queue != NULL && !isEmpty(queue) && !isFoundDest) {
		node = (Node*)headData(queue);
		tempBoard[node->yPos][node->xPos] = WALL_SQUARE;

		Direction direction;
		for (direction = 0; direction < NUM_DIRECTIONS; direction++) {
			Node* movedNode = (Node*)malloc(sizeof(Node));
			moveNode(node->xPos, node->yPos, tempBoard, direction, movedNode);

			if (movedNode != NULL) {
				if (movedNode->xPos == xPos2 && movedNode->yPos == yPos2) {
					distanceToDest = movedNode->distance;
					isFoundDest = 1;
					free(movedNode);
					break;
				}

				tempBoard[movedNode->yPos][movedNode->xPos] = WALL_SQUARE;
				append(queue, movedNode);
			}

		}

		queue = tail(queue);
	}

	freeBoard(tempBoard);
	destroyList(queueHead, free);

	return distanceToDest;
}

int evaluateBoard(Model* model) {
	int mouseXPos = model->world->mouseXPos;
	int mouseYPos = model->world->mouseYPos;
	int catXPos = model->world->catXPos;
	int catYPos = model->world->catYPos;
	int cheeseXPos = model->world->cheeseXPos;
	int cheeseYPos = model->world->cheeseYPos;

	int catMouseDist = calcDistance(catXPos, catYPos, mouseXPos, mouseYPos, model);
	int mouseCheeseDist = calcDistance(mouseXPos, mouseYPos, cheeseXPos, cheeseYPos, model);

	return (CAT_MOUSE_SCORE*catMouseDist + MOUSE_CHEESE_SCORE*mouseCheeseDist);
}

