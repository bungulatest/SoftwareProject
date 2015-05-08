#include "PlayGame.h"
#include "Widget.h"
#include "Window.h"
#include "Label.h"
#include "Button.h"
#include "Panel.h"
#include "ChooseMenu.h"
#include "Model.h"
#include "GameWindow.h"
#include "WorldFileService.h"

Bitmapfont* bitmapfont2;

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

	Widget* sidePanel = createSidePanel(windowSurface);
	addChild(window, sidePanel);

	Widget* topPanel = createTopPanel(windowSurface);
	addChild(window, topPanel);

	Widget* button1 = getWidgetFromId(BUTTON_1, window);
	setText(button1, REC_MOUSE_TEXT);

	Widget* button2 = getWidgetFromId(BUTTON_2, window);
	setText(button2, REC_CAT_TEXT);

	Widget* button3 = getWidgetFromId(BUTTON_3, window);
	setText(button3, RESTART_GAME_TEXT);

	Widget* button4 = getWidgetFromId(BUTTON_4, window);
	setText(button4, GO_TO_MAIN_MENU_TEXT);

	Widget* button5 = getWidgetFromId(BUTTON_5, window);
	setText(button5, QUIT_PROGRAM_TEXT);

	return window;
}


void playGameStart(GUI* gui, Model* initData, SDL_Surface* windowSurface) {
	gui->viewState = initializePlayGameWindow(windowSurface);
	if (initData->world != NULL) {
		gui->model = initData;
		gui->model->stateIdModel = gui->stateId;
		gui->model->markedButton = 0;
		gui->model->prevModel = initData;
	}
	else {
		World* world = createWorld(initData->worldIndex);
		gui->model = createModel(gui->stateId, initData, 0, world->firstAnimal, initData->catSkill, initData->mouseSkill, 0, initData->worldIndex);
		gui->model->world = world;
	}



	drawWidget(gui->viewState);
}


LogicEvent* playGameTranslateEvent(Widget* viewState, SDL_Event* event, Model* model) {
	if (event->type == SDL_MOUSEBUTTONUP) {
		return ChooseMenuTranslateEvent(viewState, event, model, PLAY_GAME_NUM_BUTTONS);
	}
	
	if (event->type == SDL_KEYUP) {
		if (event->key.keysym.sym == SDLK_F1) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_1, 0, 0, 0);
		}
		else if (event->key.keysym.sym == SDLK_F2) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_2, 0, 0, 0);
		}
		else if (event->key.keysym.sym == SDLK_F3) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_3, 0, 0, 0);
		}
		else if (event->key.keysym.sym == SDLK_F4) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_4, 0, 0, 0);
		}
		else if (event->key.keysym.sym == SDLK_ESCAPE) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_5, 0, 0, 0);
		}
	}

	return createLogicEvent(NO_EVENT, 0, 0, 0, 0, 0);

	
	
}


StateId playGameHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent) {
	Widget* exMarkedButton = NULL;
	Widget* newMarkedButton = NULL;
	Widget* selectedButton = NULL;
	StateId stateid;

	switch (logicalEvent->type) {

	case SELECT_BUTTON:
		selectedButton = getWidgetFromId(logicalEvent->buttonId, viewState);

		// return the appropriate StateId
		switch (selectedButton->id) {
		case BUTTON_1:
			if (model->mouseSkill == 0) {
				stateid = CHOOSE_MOUSE;
			}
			else {
				stateid = MOUSE_CHOOSE_SKILL;
			}

			break;
		case BUTTON_2:
			if (model->catSkill == 0) {
				stateid = CHOOSE_CAT;
			}
			else {
				stateid = CAT_CHOOSE_SKILL;
			}


			break;

		case BUTTON_3:

			stateid = model->stateIdModel;

			break;

		case BUTTON_4:
			stateid = MAIN_MENU;
			break;
		case BUTTON_5:
			stateid = QUIT;
			break;
		default:
			stateid = model->stateIdModel;
			break;
		}
		break;

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