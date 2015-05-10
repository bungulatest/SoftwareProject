#ifndef EvaluationService_h__
#define EvaluationService_h__

#include "Model.h"

#define CAT_MOUSE_SCORE (-10)
#define MOUSE_CHEESE_SCORE (10)
#define NUM_DIRECTIONS 4

typedef struct node {
	int xPos;
	int yPos;
	int distance; // distance from origin node, updated in BFS
} Node;
void moveNode(int xPos, int yPos, char** tempBoard, Direction direction, Node* node);
int calcDistance(int xPos1, int yPos1, int xPos2, int yPos2, Model* model);
#endif // EvaluationService_h__
