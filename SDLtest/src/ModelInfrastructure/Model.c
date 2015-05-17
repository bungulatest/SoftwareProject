#include "Model.h"
#include <stdlib.h>
#include <stdio.h>


Model* createModel(int stateId, Model* prevModel, int markedButton) {
	Model* model = (Model*)malloc(sizeof(Model));
	if (model == NULL) {
		perror("ERROR: malloc failed\n");
		exit(1);
	}
	model->stateIdModel = stateId;
	model->markedButton = markedButton;
	model->prevModel = prevModel;
	model->world = NULL;
	model->gameConfig = NULL;
	return model;
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

void freeBoard(char** board) {
	int i;
	for (i = 0; i < BOARD_GRID_SIZE; ++i) {
		free(board[i]);
	}
	free(board);
}

void freeWorld(World* world) {
	if (world->gameBoard != NULL) {
		freeBoard(world->gameBoard);
	}
	free(world);
}

void freeModel(Model* model) {
	if (model->gameConfig != NULL) {
		free(model->gameConfig);
	}
	if (model->world != NULL) {
		freeWorld(model->world);
	}
	free(model);
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