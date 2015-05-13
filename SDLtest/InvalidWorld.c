#include "InvalidWorld.h"
#include "Widget.h"
#include "Window.h"
#include "Label.h"
#include "Button.h"
#include "Panel.h"
#include "Model.h"
#include "WorldFileService.h"
#include "gui.h"
#include "ChooseMenu.h"

Bitmapfont* bitmapfont1;
Bitmapfont* bitmapfont3;
GUI* guis[STATES_COUNT];
GameValidity gameValidity;

void createGUIInvalidWorld(GUI* gui) {
	gui->start = invalidWorldStart;
	gui->viewTranslateEvent = invalidWorldTranslateEvent;
	gui->presenterHandleEvent = invalidWorldHandleEvent;
	gui->stop = invalidWorldStop;
	gui->model = NULL;
	gui->viewState = NULL;
}


Widget* initializeInvalidWorldWindow(SDL_Surface* windowSurface) {
	Widget* window = createWindow(windowSurface);

	SDL_Rect backRect = {250, 450 , 10, 10 };
	Widget* backButton = createButton(backRect, BACK_TEXT, windowSurface, REGULAR, "invalid_back.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 100, 10, BACK_BUTTON, bitmapfont3);
	addChild(window, backButton);

	SDL_Rect firstLabelRect = { 150, 100, 10, 10 };
	Widget* firstLabel = createLabel(firstLabelRect, INVALID_WORLD_TITLE_TEXT, windowSurface, 10, 10, bitmapfont3, NULL);
	addChild(window, firstLabel);

	SDL_Rect secondLabelRect = { 250, 250, 10, 10 };
	Widget* secondLabel = createLabel(secondLabelRect, NULL, windowSurface, 10, 10, bitmapfont3, NULL);
	addChild(window, secondLabel);

	if (gameValidity == CAT_IS_MISSING) {
		setText(secondLabel, CAT_IS_MISSING_TEXT);
	}
	else if (gameValidity == MOUSE_IS_MISSING) {
		setText(secondLabel, MOUSE_IS_MISSING_TEXT);
	}
	else if (gameValidity == CHEESE_IS_MISSING) {
		setText(secondLabel, CHEESE_IS_MISSING_TEXT);
	}



	return window;
}





void invalidWorldStart(GUI* gui, Model* initData, SDL_Surface* windowSurface) {
	
	gameValidity = initData->world->gameValidity;
	gui->viewState = initializeInvalidWorldWindow(windowSurface);
	

	gui->model = guis[gui->stateId]->model;
	if (gui->model == NULL) {
		gui->model = createModel(gui->stateId, initData, BACK_BUTTON);
	}
	gui->model->world = initData->world;
	gui->model->gameConfig = initData->gameConfig;
	

	drawWidget(gui->viewState);

}


LogicEvent* invalidWorldTranslateEvent(Widget* viewState, SDL_Event* event, Model* model) {

	return ChooseMenuTranslateEvent(viewState, event, model, INVALID_WORLD_NUM_BUTTONS);

}


StateId invalidWorldHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent) {
	Widget* exMarkedButton = NULL;
	Widget* newMarkedButton = NULL;
	Widget* selectedButton = NULL;
	StateId stateid = model->stateIdModel;


	switch (logicalEvent->type) {

	case SELECT_BUTTON:
		selectedButton = getWidgetFromId(logicalEvent->buttonId, viewState);

		// return the appropriate StateId
		switch (selectedButton->id) {
		case BACK_BUTTON:
			stateid = model->prevModel->stateIdModel;
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

void* invalidWorldStop(GUI* gui) {
	freeTree(gui->viewState);
	return gui->model;
}

