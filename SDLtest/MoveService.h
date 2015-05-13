#ifndef MoveService_h__
#define MoveService_h__

#include "Model.h"
#include "ListUtils.h"

#define MACHINE_DELAY 1000

World* copyWorld(World* world);
ListRef getChildWorlds(World* world);
Direction getBestMove(World* world, int catSkill, int mouseSkill);
void updateGameStatus(World* world);
Direction getDirectionFromPos(int xPos, int yPos, Model* model);

int isMoveLegal(Direction direction, World* world);

void makeMove(Direction direction, World* world);

#endif // MoveService_h__
