#ifndef MoveService_h__
#define MoveService_h__

#include "Model.h"

Direction getDirectionFromPos(int xPos, int yPos, Model* model);

int isMoveLegal(Direction direction, Model* model);

void makeMove(Direction direction, Model* model);

#endif // MoveService_h__
