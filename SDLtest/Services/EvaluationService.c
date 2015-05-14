#include "EvaluationService.h"
#include "../ModelInfrastructure/Model.h"
#include "WorldFileService.h"
#include "../main/ListUtils.h"
#include "MoveService.h"
#include "../main/MiniMax.h"
#include <stdio.h>
#include <stdlib.h>

int moveNode(int xPos, int yPos, char** tempBoard, Direction direction, Node* node) {

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
		return 0;
	}

	// square is wall
	else if (tempBoard[node->yPos][node->xPos] != EMPTY_SQUARE) {
		free(node);
		return 0;
	}
	else {
		return 1;
	}
}

int isClose(int x1, int y1, int x2, int y2) {
	return ((abs(x1 - x2) + abs(y1 - y2)) == 1);
}

int calcDistance(int xPos1, int yPos1, int xPos2, int yPos2, World* world) {
	char** tempBoard = copyBoard(world->gameBoard);
	int distanceToDest = UNREACHABLE_DISTANCE; // unreachable
	int isNodeValid = 0;

	// set the cheese square as a wall in the temp board, because the cat can't move through cheese
	tempBoard[world->cheeseYPos][world->cheeseXPos] = WALL_SQUARE;

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
			movedNode->xPos = node->xPos;
			movedNode->yPos = node->yPos;
			movedNode->distance = node->distance;
			isNodeValid = moveNode(node->xPos, node->yPos, tempBoard, direction, movedNode);

			if (isNodeValid == 1) {
				if (isClose(movedNode->xPos,movedNode->yPos, xPos2, yPos2)) {
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

int evaluateBoard(World* world) {
	int mouseXPos = world->mouseXPos;
	int mouseYPos = world->mouseYPos;
	int catXPos = world->catXPos;
	int catYPos = world->catYPos;
	int cheeseXPos = world->cheeseXPos;
	int cheeseYPos = world->cheeseYPos;

	int catMouseDist = calcDistance(catXPos, catYPos, mouseXPos, mouseYPos, world);
	int mouseCheeseDist = calcDistance(mouseXPos, mouseYPos, cheeseXPos, cheeseYPos, world);


	if (mouseXPos == 2 && mouseYPos == 2) {
		printf("eran");
	}

	if (world->gameStatus == CAT_WON){
		return MIN_EVALUATION;
	}
	if (world->gameStatus == MOUSE_WON) {
		return MAX_EVALUATION;
	}
	if (world->gameStatus == DRAW) {
		return 0;
	}

	return (CAT_MOUSE_SCORE*catMouseDist + MOUSE_CHEESE_SCORE*mouseCheeseDist);
	
}

