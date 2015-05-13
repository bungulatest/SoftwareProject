#include "MainMenu.h"
#include "gui.h"
#include "Button.h"
#include "Label.h"
#include "Widget.h"
#include "Window.h"
#include "Panel.h"
#include "Model.h"
#include "ChooseMenu.h"
#include "gui.h"

#include "GameWindow.h"

Bitmapfont* bitmapfont1;
GUI* guis[STATES_COUNT];

void createGUIMain(GUI* gui) {
	gui->start = MainStart;
	gui->viewTranslateEvent = MainTranslateEvent;
	gui->presenterHandleEvent = MainHandleEvent;
	gui->stop = MainStop;
	gui->model = NULL;
	gui->viewState = NULL;
}

Widget* initializeMainWindow(SDL_Surface* windowSurface) {
	Widget* window = createWindow(windowSurface);



	SDL_Rect panelRect = { 300, 150, 400, 600 };
	Widget* panel = createPanel(windowSurface, panelRect, SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B));
	addChild(window, panel);

	SDL_Rect labelRect = { 35, 20, 20, 20 };
	Widget* label = createLabel(labelRect, MAIN_TITLE, window->image, 0, 0, bitmapfont1, NULL);
	addChild(panel, label);

	SDL_Rect buttonNewGameRect = { 20, 60, 20, 20 };
	SDL_Rect buttonLoadGameRect = { 20, 130, 20, 20 };
	SDL_Rect buttonCreateGameRect = { 20, 200, 20, 20 };
	SDL_Rect buttonEditGameRect = { 20, 270, 20, 20 };
	SDL_Rect buttonQuitRect = { 20, 340, 20, 20 };

	Widget* buttonNewGame = createButton(buttonNewGameRect, NEW_GAME_TEXT, window->image, MARKED, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_NEW_GAME, bitmapfont1);
	addChild(panel, buttonNewGame);

	Widget* buttonLoadGame = createButton(buttonLoadGameRect, LOAD_GAME_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 25, 10, BUTTON_LOAD_GAME, bitmapfont1);
	addChild(panel, buttonLoadGame);

	Widget* buttonCreateGame = createButton(buttonCreateGameRect, CREATE_GAME_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 20, 10, BUTTON_CREATE_GAME, bitmapfont1);
	addChild(panel, buttonCreateGame);

	Widget* buttonEditGame = createButton(buttonEditGameRect, EDIT_GAME_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 30, 10, BUTTON_EDIT_GAME, bitmapfont1);
	addChild(panel, buttonEditGame);

	Widget* buttonQuit = createButton(buttonQuitRect, QUIT_TEXT, window->image, REGULAR, "button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 70, 10, BUTTON_QUIT, bitmapfont1);
	addChild(panel, buttonQuit);

	return window;
}

void MainStart(GUI* gui, Model* initData, SDL_Surface* windowSurface) {

	
	gui->viewState = initializeMainWindow(windowSurface);
	gui->model = guis[gui->stateId]->model;
	if (initData != NULL && initData->prevModel != NULL && gui->stateId == initData->prevModel->stateIdModel) {
		gui->model = initData->prevModel;
		markButtonStart(gui->model, gui->model->markedButton, BUTTON_NEW_GAME, gui->viewState);
		}
		else {
		gui->model = createModel(gui->stateId, initData, BUTTON_NEW_GAME);
		}


	drawWidget(gui->viewState);
}

LogicEvent* MainTranslateEvent(Widget* viewState, SDL_Event* event, Model* model) {

	return ChooseMenuTranslateEvent(viewState, event, model, MAIN_NUM_BUTTONS);

}


StateId MainHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent) {
	Widget* exMarkedButton = NULL;
	Widget* newMarkedButton = NULL;
	Widget* selectedButton = NULL;
	StateId stateid;

	switch (logicalEvent->type) {
	case QUIT_EVENT:
		stateid = QUIT;
		break;

	case MARK_BUTTON:
		markButton(model, logicalEvent, exMarkedButton, viewState, newMarkedButton);
		stateid = MAIN_MENU;
		break;

	case SELECT_BUTTON:
		markButton(model, logicalEvent, exMarkedButton, viewState, newMarkedButton);
		selectedButton = getWidgetFromId(logicalEvent->buttonId, viewState);

		// return the appropriate StateId
		switch (selectedButton->id) {
		case BUTTON_NEW_GAME:
			stateid = CHOOSE_CAT;
			break;
		case BUTTON_LOAD_GAME:
			stateid = LOAD_GAME;
			break;
		case BUTTON_CREATE_GAME:
			stateid = NEW_WORLD_BUILDER;
			break;
		case BUTTON_EDIT_GAME:
			stateid = EDIT_GAME;
			break;
		case BUTTON_QUIT:
			return QUIT;
			break;
		default:
			break;
		}
		break;


	default:
		stateid = model->stateIdModel;
		return stateid;
		break;
	}

	drawWidget(viewState);
	return stateid;
}

void* MainStop(GUI* gui) {
	freeTree(gui->viewState);
	return gui->model;
}