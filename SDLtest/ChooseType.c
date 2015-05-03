#include "ChooseType.h"
#include "Widget.h"
#include "Window.h"
#include "Label.h"
#include "Button.h"
#include "Panel.h"
#include "ChooseMenu.h"
#include "Model.h"


Animal currAnimal;
Player currPlayer;

void createGUIChooseType(GUI* gui) {
	gui->start = chooseTypeStart;
	gui->viewTranslateEvent = chooseTypeTranslateEvent;
	gui->presenterHandleEvent = chooseTypeHandleEvent;
	gui->stop = chooseTypeStop;
}

Widget* initializeChooseTypeWindow(SDL_Surface* windowSurface) {
	Widget* window = createWindow(windowSurface);

	SDL_Rect panelRect = { 300, 150, 400, 600 };
	Widget* panel = createPanel(windowSurface, panelRect, SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B));
	addChild(window, panel);

	SDL_Rect labelRect = { 35, 20, 20, 20 };

	Widget* label;
	if (currAnimal == CAT) {
		label = createLabel(labelRect, CHOOSE_CAT_TITLE, window->image, 0, 0);
	}
	else {
		label = createLabel(labelRect, CHOOSE_MOUSE_TITLE, window->image, 0, 0);
	}
	addChild(panel, label);

	SDL_Rect buttonHumanRect = { 20, 60, 20, 20 };
	SDL_Rect buttonMachineRect = { 20, 130, 20, 20 };
	SDL_Rect buttonBackRect = { 20, 200, 20, 20 };

	Widget* buttonHuman = createButton(buttonHumanRect, HUMAN_TEXT, window->image, MARKED, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_HUMAN);
	addChild(panel, buttonHuman);

	Widget* buttonMachine = createButton(buttonMachineRect, MACHINE_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_MACHINE);
	addChild(panel, buttonMachine);

	Widget* buttonBack = createButton(buttonBackRect, BACK_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_BACK);
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
	if (initData != NULL && initData->prevModel != NULL && gui->stateId == initData->prevModel->stateIdModel) {
		gui->model = initData->prevModel;
		markButtonStart(gui->model, gui->model->markedButton, BUTTON_HUMAN, gui->viewState);
	} 
	else {
		gui->model = createModel(gui->stateId,initData, BUTTON_HUMAN, 0, currAnimal, 0, 0, 0);
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
				if (model->world == NULL) {
					model->currAnimal = MOUSE;
					stateid = CHOOSE_MOUSE;
				}
				else {
					stateid = PLAY_GAME;
				}
			}
			else {
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
			stateid = model->prevModel->stateIdModel;
			break;
		default:
			break;
		}
		break;

	case NO_EVENT:
		stateid = model ->stateIdModel;
		return stateid;
		break;
	default:
		break;
	}

	drawWidget(viewState);
	return stateid;
}
void* chooseTypeStop(GUI* gui, StateId nextStateId) {
	freeTree(gui->viewState);
	return gui->model;
}