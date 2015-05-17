#include "ChooseType.h"
#include "../ViewInfrastructure/Widget.h"
#include "../ViewInfrastructure/Window.h"
#include "../ViewInfrastructure/Label.h"
#include "../ViewInfrastructure/Button.h"
#include "../ViewInfrastructure/Panel.h"
#include "ChooseMenu.h"
#include "../ModelInfrastructure/Model.h"
#include "../ModelInfrastructure/gui.h"

Animal currAnimal;
Player currPlayer;
Bitmapfont* bitmapfont1;
GUI* guis[STATES_COUNT];

void createGUIChooseType(GUI* gui) {
	gui->start = chooseTypeStart;
	gui->viewTranslateEvent = chooseTypeTranslateEvent;
	gui->presenterHandleEvent = chooseTypeHandleEvent;
	gui->stop = chooseTypeStop;
	gui->model = NULL;
	gui->viewState = NULL;
}

Widget* initializeChooseTypeWindow(SDL_Surface* windowSurface) {
	Widget* window = createWindow(windowSurface);

	SDL_Rect panelRect = { 300, 150, 400, 600 };
	Widget* panel = createPanel(windowSurface, panelRect, SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B));
	addChild(window, panel);

	SDL_Rect labelRect = { 35, 20, 20, 20 };

	Widget* label;
	if (currAnimal == CAT) {
		label = createLabel(labelRect, CHOOSE_CAT_TITLE, window->image, 0, 0, bitmapfont1, NULL);
	}
	else {
		label = createLabel(labelRect, CHOOSE_MOUSE_TITLE, window->image, 0, 0, bitmapfont1, NULL);
	}
	addChild(panel, label);

	SDL_Rect buttonHumanRect = { 20, 60, 20, 20 };
	SDL_Rect buttonMachineRect = { 20, 130, 20, 20 };
	SDL_Rect buttonBackRect = { 20, 200, 20, 20 };

	Widget* buttonHuman = createButton(buttonHumanRect, HUMAN_TEXT, window->image, MARKED, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_HUMAN, bitmapfont1);
	addChild(panel, buttonHuman);

	Widget* buttonMachine = createButton(buttonMachineRect, MACHINE_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_MACHINE, bitmapfont1);
	addChild(panel, buttonMachine);

	Widget* buttonBack = createButton(buttonBackRect, BACK_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_BACK, bitmapfont1);
	addChild(panel, buttonBack);

	return window;
}

void chooseTypeStart(GUI* gui, Model* initData, SDL_Surface* windowSurface) {
	if (gui->stateId == CHOOSE_CAT) {
		currAnimal = CAT;
	}
	else {
		currAnimal = MOUSE;
	}
	gui->viewState = initializeChooseTypeWindow(windowSurface);
	gui->model = guis[gui->stateId]->model;

	if (initData != NULL && initData->world != NULL) { // coming from "reconfigure cat/mouse"
		gui->model->gameConfig = initData->gameConfig;
		gui->model->world = initData->world;
		// mark the correct button
		if (currAnimal == CAT && gui->model->gameConfig->catSkill == 0) {
			markButtonStart(gui->model, BUTTON_HUMAN, BUTTON_HUMAN, gui->viewState);
		}
		else if (currAnimal == MOUSE && gui->model->gameConfig->mouseSkill == 0) {
			markButtonStart(gui->model, BUTTON_HUMAN, BUTTON_HUMAN, gui->viewState);
		}
		else {
			markButtonStart(gui->model, BUTTON_MACHINE, BUTTON_HUMAN, gui->viewState);
		}
	}

	else if (initData != NULL && initData->prevModel != NULL && gui->stateId == initData->prevModel->stateIdModel && initData->stateIdModel != PLAY_GAME) { // coming from "back" button
		gui->model = initData->prevModel;
		markButtonStart(gui->model, gui->model->markedButton, BUTTON_HUMAN, gui->viewState);
	}

	else if (initData != NULL && initData->gameConfig != NULL) { // coming from "choose cat"
		if (gui->model == NULL) { // if we come to this screen for the first time, we need to allocate the model
			gui->model = createModel(gui->stateId, initData, BUTTON_HUMAN);
		}
		else { // if this is not the first time, we don't allocate, just update the relevant fields
			gui->model->stateIdModel = gui->stateId;
			gui->model->prevModel = initData;
			gui->model->markedButton = BUTTON_HUMAN;
		}
		
		// we catch the gameConfig from the previous screen
		gui->model->gameConfig = initData->gameConfig;

		// mark the correct button
		if (currAnimal == CAT && gui->model->gameConfig->catSkill == 0) {
			gui->model->markedButton = BUTTON_HUMAN;
		}
		else if (currAnimal == MOUSE && gui->model->gameConfig->mouseSkill == 0) {
			gui->model->markedButton = BUTTON_HUMAN;
		}
		else {
			gui->model->markedButton = BUTTON_MACHINE;
		}
	}
	else { // coming from main menu
		if (gui->model == NULL) { // if we come to this screen for the first time, we need to allocate the model
			gui->model = createModel(gui->stateId, initData, BUTTON_HUMAN);
		}
		else { // if this is not the first time, we don't allocate, just update the relevant fields
			gui->model->stateIdModel = gui->stateId;
			gui->model->prevModel = initData;
			gui->model->markedButton = BUTTON_HUMAN;
		}

		gui->model->world = NULL;
		gui->model->gameConfig = createGameConfig(0, 0, 1);
		//if (gui->model->gameConfig == NULL) { // if we come to this screen for the first time, we need to allocate the game config
		//	gui->model->gameConfig = createGameConfig(0, 0, 1);
		//}
		//else { // if this is not the first time, we don't allocate, just update the relevant fields to the default values
		//	gui->model->gameConfig->catSkill = 0;
		//	gui->model->gameConfig->mouseSkill = 0;
		//	gui->model->gameConfig->worldIndex = 1;
		//}


		gui->model->markedButton = BUTTON_HUMAN;

	}
	
	
	drawWidget(gui->viewState);
}


LogicEvent* chooseTypeTranslateEvent(Widget* viewState, SDL_Event* event, Model* model) {

	return ChooseMenuTranslateEvent(viewState, event, model, CHOOSE_TYPE_NUM_BUTTONS);

}

StateId chooseTypeHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent) {
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
		case BUTTON_HUMAN:
			if (currAnimal == CAT) {
				model->gameConfig->catSkill = 0;
				if (model->world == NULL) {
					stateid = CHOOSE_MOUSE;
				}
				else {
					stateid = PLAY_GAME;
				}
			}
			else {
				model->gameConfig->mouseSkill = 0;
				stateid = PLAY_GAME;
			}
			
			break;
		case BUTTON_MACHINE:
			if (currAnimal == CAT) {
				stateid = CAT_CHOOSE_SKILL;
			}
			else {
				stateid = MOUSE_CHOOSE_SKILL;
			}
			
			break;
		case BUTTON_BACK:
			if (model->world != NULL) {
				stateid = PLAY_GAME;
			}
			else {
				stateid = model->prevModel->stateIdModel;
			}
			
			break;
		default:
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
void* chooseTypeStop(GUI* gui) {
	freeTree(gui->viewState);
	free(gui->viewState); // frees the widget struct without freeing the video surface

	return gui->model;
}