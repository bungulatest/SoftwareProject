#include "SelectWorld.h"
#include "../ViewInfrastructure/Widget.h"
#include "../ViewInfrastructure/Window.h"
#include "../ViewInfrastructure/Label.h"
#include "../ViewInfrastructure/Button.h"
#include "../ViewInfrastructure/Panel.h"
#include "ChooseMenu.h"
#include "../ModelInfrastructure/Model.h"
#include "../ModelInfrastructure/gui.h"
#include "../Services/WorldFileService.h"

Bitmapfont* bitmapfont1;

WorldSelectionWindow currSelectionWindow; // the current window(edit, save or load)
static int currWorld; //the index of the current world
GUI* guis[STATES_COUNT];

void createGuiSelectWorld(GUI* gui) {
	gui->start = selectWorldStart;
	gui->viewTranslateEvent = selectWorldTranslateEvent;
	gui->presenterHandleEvent = selectWorldHandleEvent;
	gui->stop = selectWorldStop;
	gui->model = NULL;
	gui->viewState = NULL;
}

Widget* initializeChooseWorldWindow(SDL_Surface* windowSurface) {
	Widget* window = createWindow(windowSurface);
	char stringBuffer[SELECT_WORLD_STRING_LENGTH];
	char digitBuffer[2];
	strcpy(stringBuffer, SELECT_WORLD_COOSE_TEXT);

	SDL_Rect panelRect = { 300, 150, 400, 600 };
	Widget* panel = createPanel(windowSurface, panelRect, SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B));
	addChild(window, panel);

	SDL_Rect labelRect = { 35, 20, 20, 20 };

	Widget* label;
	if (currSelectionWindow == LOAD) {
		label = createLabel(labelRect, LOAD_GAME_TITLE, window->image, 0, 0, bitmapfont1, NULL);
	}
	else if (currSelectionWindow == EDIT) {
		label = createLabel(labelRect, EDIT_GAME_TITLE, window->image, 0, 0, bitmapfont1, NULL);
	}
	else {
		label = createLabel(labelRect, SAVE_GAME_TITLE, window->image, 0, 0, bitmapfont1, NULL);
	}
	addChild(panel, label);

	SDL_Rect buttonSelectRect = { 20, 60, 20, 20 };
	SDL_Rect buttonDoneRect = { 20, 130, 20, 20 };
	SDL_Rect buttonBackRect = { 20, 200, 20, 20 };
	SDL_Rect buttonUpRect = { 186, 60, 20, 20 };
	SDL_Rect buttonDownRect = { 186, 84, 20, 20 };


	sprintf(digitBuffer, "%d", currWorld);
	strcat(stringBuffer, digitBuffer);

	Widget* buttonSelect = createButton(buttonSelectRect, stringBuffer, window->image, MARKED, "select_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_SELECT_WORLD, bitmapfont1);
	addChild(panel, buttonSelect);

	Widget* buttonDone = createButton(buttonDoneRect, SELECT_WORLD_DONE_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_SELECT_WORLD_DONE, bitmapfont1);
	addChild(panel, buttonDone);

	Widget* buttonBack = createButton(buttonBackRect, SELECT_WORLD_BACK_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_SELECT_WORLD_BACK, bitmapfont1);
	addChild(panel, buttonBack);

	Widget* buttonUp = createButton(buttonUpRect, NULL, window->image, REGULAR, "up_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_SELECT_WORLD_INCREASE, bitmapfont1);
	addChild(panel, buttonUp);

	Widget* buttonDown = createButton(buttonDownRect, NULL, window->image, REGULAR, "down_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_SELECT_WORLD_DECREASE, bitmapfont1);
	addChild(panel, buttonDown);

	return window;
}

void changeWorldButton(Widget* viewState) {
	char digitBuffer[2]; // holds the skill level string
	char stringBuffer[SELECT_WORLD_STRING_LENGTH];
	strcpy(stringBuffer, SELECT_WORLD_COOSE_TEXT);
	sprintf(digitBuffer, "%d", currWorld);
	strcat(stringBuffer, digitBuffer);
	Widget* worldlButton = getWidgetFromId(BUTTON_SELECT_WORLD, viewState);
	setText(worldlButton, stringBuffer);
}

void selectWorldStart(GUI* gui, Model* initData, SDL_Surface* windowSurface) {
	if (gui->stateId == EDIT_GAME) {
		currSelectionWindow = EDIT;
	}
	else if (gui->stateId == SAVE_GAME) {
		currSelectionWindow = SAVE;
	}
	else {
		currSelectionWindow = LOAD;
	}


	char stringBuffer[SELECT_WORLD_STRING_LENGTH];
	char digitBuffer[2];
	strcpy(stringBuffer, SELECT_WORLD_COOSE_TEXT);

	gui->viewState = initializeChooseWorldWindow(windowSurface);
	gui->model = guis[gui->stateId]->model;
	if (initData != NULL && initData->prevModel != NULL && gui->stateId == initData->prevModel->stateIdModel) {// coming from "back" button
		Widget* worldButton = getWidgetFromId(BUTTON_SELECT_WORLD, gui->viewState);


		gui->model = initData->prevModel;

		currWorld = gui->model->gameConfig->worldIndex;

		sprintf(digitBuffer, "%d", currWorld);
		strcat(stringBuffer, digitBuffer);

		markButtonStart(gui->model, gui->model->markedButton, BUTTON_SELECT_WORLD, gui->viewState);
		setText(worldButton, stringBuffer);
	}

	else if (initData != NULL && initData->gameConfig != NULL) { // coming from "world builder"
		if (gui->model == NULL) {
			gui->model = createModel(gui->stateId, initData, BUTTON_SELECT_WORLD);
		}
		gui -> model->world = initData->world;
		gui->model->gameConfig = initData->gameConfig;
		currWorld = gui->model->gameConfig->worldIndex;

		sprintf(digitBuffer, "%d", currWorld);
		strcat(stringBuffer, digitBuffer);

		Widget* worldButton = getWidgetFromId(BUTTON_SELECT_WORLD, gui->viewState);
		markButtonStart(gui->model, BUTTON_SELECT_WORLD, BUTTON_SELECT_WORLD, gui->viewState);
		setText(worldButton, stringBuffer);
	}
	else { // coming from main menu
		if (gui->model == NULL) { // if we come to this screen for the first time, we need to allocate the model
			gui->model = createModel(gui->stateId, initData, BUTTON_SELECT_WORLD);
		}
		else { // if this is not the first time, we don't allocate, just update the relevant fields
			gui->model->stateIdModel = gui->stateId;
			gui->model->prevModel = initData;
			gui->model->markedButton = BUTTON_SELECT_WORLD;
		}


		if (gui->model->gameConfig == NULL) { // if we come to this screen for the first time, we need to allocate the game config
			gui->model->gameConfig = createGameConfig(0, 0, DEFAULT_WORLD);
		}
		else { // if this is not the first time, we don't allocate, just update the relevant fields to the default values
			gui->model->gameConfig->catSkill = 0;
			gui->model->gameConfig->mouseSkill = 0;
			gui->model->gameConfig->worldIndex = DEFAULT_WORLD;
		}

		Widget* worldButton = getWidgetFromId(BUTTON_SELECT_WORLD, gui->viewState);
		currWorld = DEFAULT_WORLD;

		sprintf(digitBuffer, "%d", currWorld);
		strcat(stringBuffer, digitBuffer);
		setText(worldButton, stringBuffer);

		markButtonStart(gui->model, BUTTON_SELECT_WORLD, BUTTON_SELECT_WORLD, gui->viewState);
	}



	drawWidget(gui->viewState);
}

LogicEvent* selectWorldTranslateUpOrDownEvent(Widget* viewState, SDL_Event* event, Model* model) {

	switch (event->key.keysym.sym) {
	case (SDLK_UP) :
		return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_SELECT_WORLD_INCREASE, 0, 0, 0);
		break;
	case (SDLK_DOWN) :
		return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_SELECT_WORLD_DECREASE, 0, 0, 0);
		break;

	default:
		return createLogicEvent(OTHER_EVENT, 0, 0, 0, 0, 0);
		break;
	}
	createLogicEvent(OTHER_EVENT, 0, 0, 0, 0, 0);
}

LogicEvent* selectWorldTranslateEvent(Widget* viewState, SDL_Event* event, Model* model) {

	if ((event->type == SDL_KEYUP) && (event->key.keysym.sym == SDLK_UP || event->key.keysym.sym == SDLK_DOWN)) {
		return selectWorldTranslateUpOrDownEvent(viewState, event, model);
	}
	else {
		return ChooseMenuTranslateEvent(viewState, event, model, SELECT_WORLD_NUM_BUTTONS);
	}


}

StateId selectWorldHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent) {
	Widget* exMarkedButton = NULL;
	Widget* newMarkedButton = NULL;
	Widget* selectedButton = NULL;
	StateId stateid;

	switch (logicalEvent->type) {
	case QUIT_EVENT:
		stateid = QUIT;
		free(logicalEvent);
		return stateid;
		break;

	case MARK_BUTTON:
		markButton(model, logicalEvent, exMarkedButton, viewState, newMarkedButton);
		stateid = model->stateIdModel;
		break;

	case SELECT_BUTTON:
		markButton(model, logicalEvent, exMarkedButton, viewState, newMarkedButton);
		selectedButton = getWidgetFromId(logicalEvent->buttonId, viewState);

		// return the appropriate StateId
		switch (selectedButton->id) {
		case BUTTON_SELECT_WORLD_DONE:
			model->gameConfig->worldIndex = currWorld;
			if (currSelectionWindow == LOAD) {
				stateid = CHOOSE_CAT;
			}
			else if (currSelectionWindow == EDIT) {
				stateid = EDIT_WORLD_BUILDER;
			}
			else {
				saveWorldToFile(model->gameConfig->worldIndex, model->world);
				stateid = EDIT_WORLD_BUILDER;
			}

			break;
		case BUTTON_SELECT_WORLD_INCREASE:
			markButtonStart(model, BUTTON_SELECT_WORLD, BUTTON_SELECT_WORLD, viewState);
			if (currWorld < MAX_WORLD) {
				currWorld++;
				changeWorldButton(viewState);
			}
			stateid = model->stateIdModel;

			break;

		case BUTTON_SELECT_WORLD_DECREASE:
			markButtonStart(model, BUTTON_SELECT_WORLD, BUTTON_SELECT_WORLD, viewState);
			if (currWorld > MIN_WORLD) {
				currWorld--;
				changeWorldButton(viewState);
			}
			stateid = model->stateIdModel;

			break;

		case BUTTON_SELECT_WORLD_BACK:
			stateid = model->prevModel->stateIdModel;
			break;
		default:
			stateid = model->stateIdModel;
			break;
		}
		break;

	default:
		stateid = model->stateIdModel;
		free(logicalEvent);
		return stateid;
		break;
	}

	drawWidget(viewState);


	free(logicalEvent);
	return stateid;
}

void* selectWorldStop(GUI* gui) {
	freeTree(gui->viewState);
	free(gui->viewState); // frees the widget struct without freeing the video surface
	return gui->model;
}