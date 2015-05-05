#include "Model.h"
#include <stdlib.h>


Model* createModel(int stateId,Model* prevModel, int markedButton, Player currPlayer, Animal currAnimal, int catSkill, int mouseSkill, int isPaused, int worldIndex) {
	Model* model = (Model*)malloc(sizeof(Model));
	if (model == NULL) {
		//TODO: handle error
		perror("ERROR: malloc failed");
		return NULL;
	}
	model->stateIdModel = stateId;
	model->catSkill = catSkill;
	model->mouseSkill = mouseSkill;
	model->currPlayer = currPlayer;
	model->currAnimal = currAnimal;
	model->isPaused = isPaused;
	model->markedButton = markedButton;
	model->prevModel = prevModel;
	model->world = NULL;
	model->worldIndex = worldIndex;
	return model;
}

void freeModel(Model* model) {
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