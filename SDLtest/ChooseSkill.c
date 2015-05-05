#include "ChooseSkill.h"
#include "Widget.h"
#include "Window.h"
#include "Label.h"
#include "Button.h"
#include "Panel.h"
#include "ChooseMenu.h"
#include "Model.h"


Animal currAnimal;
Player currPlayer;
static int currSkill;

void createGUIChooseSkill(GUI* gui) {
	gui->start = chooseSkillStart;
	gui->viewTranslateEvent = chooseSkillTranslateEvent;
	gui->presenterHandleEvent = chooseSkillHandleEvent;
	gui->stop = chooseSkillStop;
}


Widget* initializeChooseSkillWindow(SDL_Surface* windowSurface) {
	Widget* window = createWindow(windowSurface);
	char buffer[2];

	SDL_Rect panelRect = { 300, 150, 400, 600 };
	Widget* panel = createPanel(windowSurface, panelRect, SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B));
	addChild(window, panel);

	SDL_Rect labelRect = { 35, 20, 20, 20 };

	Widget* label;
	if (currAnimal == CAT) {
		label = createLabel(labelRect, CHOOSE_CAT_SKILL_TITLE, window->image, 0, 0);
	}
	else {
		label = createLabel(labelRect, CHOOSE_MOUSE_SKILL_TITLE, window->image, 0, 0);
	}
	addChild(panel, label);

	SDL_Rect buttonSelectRect = { 20, 60, 20, 20 };
	SDL_Rect buttonDoneRect = { 20, 130, 20, 20 };
	SDL_Rect buttonBackRect = { 20, 200, 20, 20 };
	SDL_Rect buttonUpRect = { 186, 60, 20, 20 };
	SDL_Rect buttonDownRect = { 186, 84, 20, 20 };

	_itoa(currSkill, buffer, 10); //TODO: check if _itoa works in linux

	Widget* buttonSelect = createButton(buttonSelectRect, buffer, window->image, MARKED, "select_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_SKILL_LEVEL);
	addChild(panel, buttonSelect);

	Widget* buttonDone = createButton(buttonDoneRect, SKILL_DONE_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_SKILL_DONE);
	addChild(panel, buttonDone);

	Widget* buttonBack = createButton(buttonBackRect, SKILL_BACK_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_SKILL_BACK);
	addChild(panel, buttonBack);

	Widget* buttonUp = createButton(buttonUpRect, NULL, window->image, REGULAR, "up_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_SKILL_INCREASE);
	addChild(panel, buttonUp);

	Widget* buttonDown = createButton(buttonDownRect, NULL, window->image, REGULAR, "down_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_SKILL_DECREASE);
	addChild(panel, buttonDown);

	return window;
}

void changeSkillButton(Widget* viewState) {
	char buffer[2]; // holds the skill level string
	_itoa(currSkill, buffer, 10);
	Widget* skillButton = getWidgetFromId(BUTTON_SKILL_LEVEL, viewState);
	setText(skillButton, buffer);
}

void chooseSkillStart(GUI* gui, Model* initData, SDL_Surface* windowSurface) {
	if (gui->stateId == CAT_CHOOSE_SKILL) {
		currAnimal = CAT;
	}
	else {
		currAnimal = MOUSE;
	}


	char buffer[2];
	gui->viewState = initializeChooseSkillWindow(windowSurface);
	if (initData != NULL && initData->prevModel != NULL && gui->stateId == initData->prevModel->stateIdModel) {
		Widget* skillButton = getWidgetFromId(BUTTON_SKILL_LEVEL, gui->viewState);
		
		
		gui->model = initData->prevModel;

		if (gui->model->currAnimal == CAT) {
			currSkill = gui->model->catSkill;
		}
		else {
			currSkill = gui->model->mouseSkill;
		}
		_itoa(currSkill, buffer, 10);
		
		markButtonStart(gui->model, gui->model->markedButton, BUTTON_SKILL_LEVEL, gui->viewState);
		setText(skillButton, buffer);
	}
	else {
		gui->model = createModel(gui->stateId, initData, BUTTON_SKILL_LEVEL, 0, currAnimal, 0, 0, 0, 1);
		Widget* skillButton = getWidgetFromId(BUTTON_SKILL_LEVEL, gui->viewState);
		currSkill = DEFAULT_SKILL;
		_itoa(currSkill, buffer, 10);
		setText(skillButton, buffer);
		
	}

	

	drawWidget(gui->viewState);
}

LogicEvent* chooseSkillTranslateUpOrDownEvent(Widget* viewState, SDL_Event* event, Model* model) {

	switch (event->key.keysym.sym) {
	case (SDLK_UP) :
		return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_SKILL_INCREASE, 0, 0, 0);
		break;
	case (SDLK_DOWN) :
		return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_SKILL_DECREASE, 0, 0, 0);
		break;

	default:
		return createLogicEvent(NO_EVENT, 0, 0, 0, 0, 0);
		break;
	}
}

LogicEvent* chooseSkillTranslateEvent(Widget* viewState, SDL_Event* event, Model* model) {

	if ((event->type == SDL_KEYUP) && (event->key.keysym.sym == SDLK_UP || event->key.keysym.sym == SDLK_DOWN)) {
		return chooseSkillTranslateUpOrDownEvent(viewState, event, model);
	}
	else {
		return ChooseMenuTranslateEvent(viewState, event, model, CHOOSE_SKILL_NUM_BUTTONS);
	}
	

}


StateId chooseSkillHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent) {
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
		case BUTTON_SKILL_DONE:
			if (currAnimal == CAT) {
				model->catSkill = currSkill;
				if (model->world == NULL) {
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
		case BUTTON_SKILL_INCREASE:
			markButtonStart(model, BUTTON_SKILL_LEVEL, BUTTON_SKILL_LEVEL, viewState);
			if (currSkill < MAX_SKILL) {
				currSkill++;
				changeSkillButton(viewState);
			}
			stateid = model->stateIdModel;

			break;

		case BUTTON_SKILL_DECREASE:
			markButtonStart(model, BUTTON_SKILL_LEVEL, BUTTON_SKILL_LEVEL, viewState);
			if (currSkill > MIN_SKILL) {
				currSkill--;
				changeSkillButton(viewState);
			}
			stateid = model->stateIdModel;

			break;

		case BUTTON_SKILL_BACK:
			stateid = model->prevModel->stateIdModel;
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

void* chooseSkillStop(GUI* gui, StateId nextStateId) {
	freeTree(gui->viewState);
	return gui->model;
}