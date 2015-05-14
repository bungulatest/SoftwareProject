#include "Model.h"
#include <stdlib.h>


Model* createModel(int stateId, Model* prevModel, int markedButton) {
	Model* model = (Model*)malloc(sizeof(Model));
	if (model == NULL) {
		//TODO: handle error
		perror("ERROR: malloc failed");
		return NULL;
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
		//TODO: handle error
		perror("ERROR: malloc failed");
		return NULL;
	}
	gameConfig->catSkill = catSkill;
	gameConfig->mouseSkill = mouseSkill;
	gameConfig->worldIndex = worldIndex;
	return gameConfig;
}

void freeModel(Model* model) {
	//TODO: free World
	if (model->gameConfig != NULL) {
		free(model->gameConfig);
	}
	free(model);
}

LogicEvent* createLogicEvent(EventType type, int markedButton, int buttonId, int row, int col, Direction direction) {
	LogicEvent* event = (LogicEvent*)malloc(sizeof(LogicEvent));

	event->type = type;
	event->markedId = markedButton;
	event->buttonId = buttonId;
	event->row = row;
	event->col = col;
	event->direction = direction;

	return event;
}