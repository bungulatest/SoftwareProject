#ifndef MoveService_h__
#define MoveService_h__

#include "Model.h"
#include "ListUtils.h"

World* copyWorld(World* world);
ListRef getChildWorlds(World* world);
void updateGameStatus(World* world);
Direction getDirectionFromPos(int xPos, int yPos, Model* model);

int isMoveLegal(Direction direction, World* world);

void makeMove(Direction direction, World* world);

#endif // MoveService_h__
