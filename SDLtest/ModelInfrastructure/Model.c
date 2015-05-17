#include "Model.h"
#include "../Services/WorldFileService.h"
#include <stdlib.h>
#include <stdio.h>


Model* createModel(int stateId, Model* prevModel, int markedButton) {
	Model* model = (Model*)malloc(sizeof(Model));
	if (model == NULL) {
		perror("ERROR: malloc failed");
		exit(1);
	}
	model->stateIdModel = stateId;
	model->markedButton = markedButton;
	model->prevModel = prevModel;
	model->world = NULL;
	model->gameConfig = NULL;
	return model;
}

void updateModelFromModel(Model* sourceModel, Model* destModel) {
	if (sourceModel->gameConfig != NULL) {
		destModel->gameConfig = updateGameConfigFromGameConfig(sourceModel->gameConfig, destModel->gameConfig);
	}
	else {
		destModel->gameConfig = createGameConfig(0, 0, 1);
	}

	if (sourceModel->world != NULL) {
		destModel->world = updateWorldFromWorld(sourceModel->world, destModel->world);
	}
	else {
		
		//destModel->world = createEmptyWorld();
	}

}

GameConfig* updateGameConfigFromGameConfig(GameConfig* sourceConfig, GameConfig* destConfig) {
	if (destConfig == NULL) { // gameConfig is not allocated yet
		destConfig = createGameConfig(0, 0, 1);
	}
	destConfig->catSkill = sourceConfig->catSkill;
	destConfig->mouseSkill = sourceConfig->mouseSkill;
	destConfig->worldIndex = sourceConfig->worldIndex;
	return destConfig;
}

World* updateWorldFromWorld(World* sourceWorld, World* destWorld) {
	if (destWorld == NULL) { // World is not allocated yet
		destWorld = createEmptyWorld();
	}
	destWorld->catXPos = sourceWorld->catXPos;
	destWorld->catYPos = sourceWorld->catYPos;
	destWorld->mouseXPos = sourceWorld->mouseXPos;
	destWorld->mouseYPos = sourceWorld->mouseYPos;
	destWorld->cheeseXPos = sourceWorld->cheeseXPos;
	destWorld->cheeseYPos = sourceWorld->cheeseYPos;
	destWorld->currAnimal = sourceWorld->currAnimal;
	destWorld->totalTurns = sourceWorld->totalTurns;
	destWorld->isGameOver = sourceWorld->isGameOver;
	destWorld->isPaused = sourceWorld->isPaused;
	destWorld->currTurn = sourceWorld->currTurn;
	destWorld->firstAnimal = sourceWorld->firstAnimal;
	destWorld->gameStatus = sourceWorld->gameStatus;
	destWorld->gameValidity = sourceWorld->gameValidity;

	if (destWorld->gameBoard == NULL && sourceWorld->gameBoard != NULL) { // we need to allocate the game board
		destWorld->gameBoard = copyBoard(sourceWorld->gameBoard);
	}
	if (destWorld->gameBoard != NULL && sourceWorld->gameBoard != NULL) { // we don't need to allocate, just update
		updateBoard(sourceWorld->gameBoard, destWorld->gameBoard);
	}
	return destWorld;

}

void freeWorld(World* world) {
	if (world == NULL) {
		return;
	}
	if (world->gameBoard != NULL) {
		freeBoard(world->gameBoard);
	}
	free(world);
}

GameConfig* createGameConfig(int catSkill, int mouseSkill, int worldIndex) {
	GameConfig* gameConfig = (GameConfig*)malloc(sizeof(GameConfig));
	if (gameConfig == NULL) {
		perror("ERROR: malloc failed\n");
		exit(1);
	}
	gameConfig->catSkill = catSkill;
	gameConfig->mouseSkill = mouseSkill;
	gameConfig->worldIndex = worldIndex;
	return gameConfig;
}




LogicEvent* createLogicEvent(EventType type, int markedButton, int buttonId, int row, int col, Direction direction) {
	LogicEvent* event = (LogicEvent*)malloc(sizeof(LogicEvent));
	if (event == NULL) {
		perror("ERROR: malloc failed\n");
		exit(1);
	}
	event->type = type;
	event->markedId = markedButton;
	event->buttonId = buttonId;
	event->row = row;
	event->col = col;
	event->direction = direction;

	return event;
}