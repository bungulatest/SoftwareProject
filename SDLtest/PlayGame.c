#include "PlayGame.h"
#include "Widget.h"
#include "Window.h"
#include "Label.h"
#include "Button.h"
#include "Panel.h"
#include "ChooseMenu.h"
#include "Model.h"
#include "GameWindow.h"


void createGUIPlayGame(GUI* gui) {
	gui->start = playGameStart;
	gui->viewTranslateEvent = playGameTranslateEvent;
	gui->presenterHandleEvent = playGameHandleEvent;
	gui->stop = playGameStop;
}


Widget* initializePlayGameWindow(SDL_Surface* windowSurface) {
	Widget* window = createWindow(windowSurface);

	Widget* gridPanel = createGridFromModel(NULL, windowSurface);
	addChild(window, gridPanel);

	return window;
}


void playGameStart(GUI* gui, Model* initData, SDL_Surface* windowSurface) {
	


	gui->viewState = initializePlayGameWindow(windowSurface);
	if (initData != NULL && initData->prevModel != NULL && gui->stateId == initData->prevModel->stateIdModel) {
	}
	else {
		gui->model = createModel(gui->stateId, initData, 0, 0, 0, 0, 0, 0, 1);
	}



	drawWidget(gui->viewState);
}


LogicEvent* playGameTranslateEvent(Widget* viewState, SDL_Event* event, Model* model) {
	return createLogicEvent(NO_EVENT, 0, 0, 0, 0, 0);
}


StateId playGameHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent) {
	Widget* exMarkedButton = NULL;
	Widget* newMarkedButton = NULL;
	Widget* selectedButton = NULL;
	StateId stateid = 0;

	switch (logicalEvent->type) {

	case NO_EVENT:
		stateid = model->stateIdModel;
		return stateid;
		break;
	default:
		break;
	}

	drawWidget(viewState);
	return stateid;
}

void* playGameStop(GUI* gui, StateId nextStateId) {
	freeTree(gui->viewState);
	return gui->model;
}