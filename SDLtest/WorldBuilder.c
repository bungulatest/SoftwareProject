#include "WorldBuilder.h"
#include "Widget.h"
#include "Window.h"
#include "Label.h"
#include "Button.h"
#include "Panel.h"
#include "ChooseMenu.h"
#include "Model.h"
#include "gui.h"
#include "GameWindow.h"
#include "MoveService.h"
#include "SelectWorld.h"
#include "WorldFileService.h"



Bitmapfont* bitmapfont1;
Bitmapfont* bitmapfont2;

WorldBuilderWindow currWorldBuilderWindow; // the current window(new world or edit world)
int currWorldIndex;

GUI* guis[STATES_COUNT];

void createGUIWorldBuilder(GUI* gui) {
	gui->start = worldBuilderStart;
	gui->viewTranslateEvent = worldBuilderTranslateEvent;
	gui->presenterHandleEvent = worldBuilderHandleEvent;
	gui->stop = worldBuilderStop;
	gui->model = NULL;
	gui->viewState = NULL;
}

Widget* initializeWorldBuilderWindow(SDL_Surface* windowSurface) {
	Widget* window = createWindow(windowSurface);

	Widget* gridPanel = createEmptyGrid(windowSurface);
	addChild(window, gridPanel);

	Widget* sidePanel = createSidePanel(windowSurface);
	addChild(window, sidePanel);

	Widget* topPanel = createTopPanel(windowSurface);
	addChild(window, topPanel);

	Widget* button1 = getWidgetFromId(BUTTON_1, window);
	setText(button1, PLACE_MOUSE_TEXT);

	Widget* button2 = getWidgetFromId(BUTTON_2, window);
	setText(button2, PLACE_CAT_TEXT);

	Widget* button3 = getWidgetFromId(BUTTON_3, window);
	setText(button3, PLACE_CHEESE_TEXT);

	Widget* button4 = getWidgetFromId(BUTTON_4, window);
	setText(button4, PLACE_WALL_TEXT);

	Widget* button5 = getWidgetFromId(BUTTON_5, window);
	setText(button5, PLACE_EMPTY_SPACE_TEXT);

	char stringBuffer[STRING_MAX_LENGTH];
	char digitBuffer[3];
	if (currWorldBuilderWindow == NEW_WORLD_BUILDER_WINDOW) {
		strcpy(stringBuffer, NEW_WORLD_TITLE);
	}
	else {
		strcpy(stringBuffer, EDIT_WORLD_TITLE);
		_itoa(currWorldIndex, digitBuffer, 10);
		strcat(stringBuffer, digitBuffer);
	}

	SDL_Rect titleRect = { 320, 20, 20, 20 };

	Widget* titleLabel = createLabel(titleRect, stringBuffer, windowSurface, 10, 10, bitmapfont1, NULL);
	addChild(topPanel, titleLabel);

	SDL_Rect saveWorldRect = { 18, 130, 200, 50 };
	Widget* saveWorldButton = createButton(saveWorldRect, SAVE_WORLD_TEXT, windowSurface, REGULAR, "side_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 10, 10, SIDE_NUM_BUTTONS, bitmapfont2);
	addChild(topPanel, saveWorldButton);
	
	SDL_Rect goToMainMenuRect = { 306, 130, 200, 50 };
	Widget* goToMainMenuButton = createButton(goToMainMenuRect, GO_TO_MAIN_MENU_TEXT_F1, windowSurface, REGULAR, "side_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 10, 10, SIDE_NUM_BUTTONS + 1, bitmapfont2);
	addChild(topPanel, goToMainMenuButton);

	SDL_Rect quitProgramRect = { 602, 130, 200, 50 };
	Widget* quitProgramButton = createButton(quitProgramRect, QUIT_PROGRAM_TEXT_ESC, windowSurface, REGULAR, "side_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 10, 10, SIDE_NUM_BUTTONS + 2, bitmapfont2);
	addChild(topPanel, quitProgramButton);

	return window;
	
}


void worldBuilderStart(GUI* gui, Model* initData, SDL_Surface* windowSurface) {

	if (gui->stateId == NEW_WORLD_BUILDER) {
		currWorldBuilderWindow = NEW_WORLD_BUILDER_WINDOW;
	}
	else {
		currWorldBuilderWindow = EDIT_WORLD_BUILDER_WINDOW;
	}


	

	gui->model = guis[gui->stateId]->model;

	if (initData->world != NULL) { // coming from "back" button on "save world" or "done button" on "save world"
		if (gui->model == NULL) {
			gui->model = createModel(gui->stateId, initData, MAX_NUM_BUTTONS);
		}
		gui->model->gameConfig = initData->gameConfig;
		gui->model->world = initData->world;
		gui->model->markedButton = MAX_NUM_BUTTONS;
	}
	else if (initData->gameConfig != NULL) { // coming from "done" button on "edit world"
		World* world = createWorld(initData->gameConfig->worldIndex);
		gui->model = createModel(gui->stateId, initData, 0);
		gui->model->gameConfig = initData->gameConfig;
		gui->model->world = world;
		gui->model->markedButton = MAX_NUM_BUTTONS;
	}
	else { // coming from "create world" on main menu
		World* world = createEmptyWorld();
		gui->model = createModel(gui->stateId, initData, 0);
		gui->model->gameConfig = createGameConfig(0,0,DEFAULT_WORLD);
		gui->model->world = world;
		gui->model->markedButton = MAX_NUM_BUTTONS;
	}

	if (initData != NULL && initData->prevModel != NULL && gui->stateId == initData->prevModel->stateIdModel) { // remember the marked square when coming from "back"
		gui->model->markedButton = initData->prevModel->markedButton;
	}

	if (gui->model->gameConfig != NULL) {
		currWorldIndex = gui->model->gameConfig->worldIndex;
	}
	
	gui->viewState = initializeWorldBuilderWindow(windowSurface);

	markButtonStart(gui->model, gui->model->markedButton, MAX_NUM_BUTTONS, gui->viewState);

	updateGrid(gui->model, gui->viewState);

	drawWidget(gui->viewState);
	


}

LogicEvent* worldBuilderTranslateEvent(Widget* viewState, SDL_Event* event, Model* model) {
	if (event->type == SDL_MOUSEBUTTONUP) {
		return ChooseMenuTranslateEvent(viewState, event, model, WORLD_BUILDER_NUM_BUTTONS);
	}

	if (event->type == SDL_KEYUP) {

		// side panel buttons
		if (event->key.keysym.sym == SDLK_m) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_1, 0, 0, 0);
		}
		else if (event->key.keysym.sym == SDLK_c) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_2, 0, 0, 0);
		}
		else if (event->key.keysym.sym == SDLK_p) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_3, 0, 0, 0);
		}
		else if (event->key.keysym.sym == SDLK_w) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_4, 0, 0, 0);
		}
		else if (event->key.keysym.sym == SDLK_SPACE) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, BUTTON_5, 0, 0, 0);
		}
		//save world
		else if (event->key.keysym.sym == SDLK_s) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, SIDE_NUM_BUTTONS, 0, 0, 0);
		}
		// go to main menu
		else if (event->key.keysym.sym == SDLK_F1) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, SIDE_NUM_BUTTONS+1, 0, 0, 0);
		}
		//quit
		else if (event->key.keysym.sym == SDLK_ESCAPE) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, SIDE_NUM_BUTTONS+2, 0, 0, 0);
		}

		// keyboard player move
		else if (event->key.keysym.sym == SDLK_UP) {
			return createLogicEvent(MOVE_PLAYER_TO_DIRECTION, 0, 0, 0, 0, UP);
		}
		else if (event->key.keysym.sym == SDLK_DOWN) {
			return createLogicEvent(MOVE_PLAYER_TO_DIRECTION, 0, 0, 0, 0, DOWN);
		}
		else if (event->key.keysym.sym == SDLK_RIGHT) {
			return createLogicEvent(MOVE_PLAYER_TO_DIRECTION, 0, 0, 0, 0, RIGHT);
		}
		else if (event->key.keysym.sym == SDLK_LEFT) {
			return createLogicEvent(MOVE_PLAYER_TO_DIRECTION, 0, 0, 0, 0, LEFT);
		}
	}

	return createLogicEvent(OTHER_EVENT, 0, 0, 0, 0, 0);
}
void markedButtonByDirection(Model* model, Direction direction, Widget* viewState) {
	int newButtonId = model->markedButton;
	if (direction == UP) {
		newButtonId -= BOARD_SIZE;
	}
	if (direction == DOWN) {
		newButtonId += BOARD_SIZE;
	}
	if (direction == LEFT && ((newButtonId - MAX_NUM_BUTTONS) % BOARD_SIZE) != 0) {
		newButtonId--;
	}
	if (direction == RIGHT && ((newButtonId - MAX_NUM_BUTTONS) % BOARD_SIZE) != (BOARD_SIZE - 1)) {
		newButtonId++;
	}
	if (!(newButtonId < MAX_NUM_BUTTONS || newButtonId >(MAX_NUM_BUTTONS + BOARD_SIZE*BOARD_SIZE))) {
		markButtonStart(model, newButtonId, model->markedButton, viewState);
	}
}

void clearObjectFromWorld(World* world, int xPos, int yPos) {
	if (world->gameBoard[yPos][xPos] == WALL_SQUARE) {
		world->gameBoard[yPos][xPos] = EMPTY_SQUARE;
	}
	if (world->catXPos == xPos && world->catYPos == yPos) {
		world->catXPos = -1;
		world->catYPos = -1;
	}
	if (world->mouseXPos == xPos && world->mouseYPos == yPos) {
		world->mouseXPos = -1;
		world->mouseYPos = -1;
	}
	if (world->cheeseXPos == xPos && world->cheeseYPos == yPos) {
		world->cheeseXPos = -1;
		world->cheeseYPos = -1;
	}
	return;
}

StateId worldBuilderHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent) {
	Widget* exMarkedButton = NULL;
	Widget* newMarkedButton = NULL;
	Widget* selectedButton = NULL;
	GameValidity gameValidity;
	StateId stateid = model->stateIdModel;
	int xPos, yPos ;
	Direction direction = -1;
	int moveLegal = 0;

	switch (logicalEvent->type) {

	case SELECT_BUTTON:
		selectedButton = getWidgetFromId(logicalEvent->buttonId, viewState);
		xPos = (model ->markedButton- MAX_NUM_BUTTONS) % BOARD_SIZE;
		yPos = ((model ->markedButton - MAX_NUM_BUTTONS) - xPos) / BOARD_SIZE;
		
		
		// return the appropriate StateId
		switch (selectedButton->id) {
		case BUTTON_1:
			clearObjectFromWorld(model->world, xPos, yPos);
			model->world->mouseXPos = xPos;
			model->world->mouseYPos = yPos;
			updateGrid(model, viewState);
			model->world->firstAnimal = MOUSE;

			break;
		case BUTTON_2:

			clearObjectFromWorld(model->world, xPos, yPos);
			model->world->catXPos = xPos;
			model->world->catYPos = yPos;
			updateGrid(model, viewState);
			model->world->firstAnimal = CAT;

			break;

		case BUTTON_3:

			clearObjectFromWorld(model->world, xPos, yPos);
			model->world->cheeseXPos = xPos;
			model->world->cheeseYPos = yPos;
			updateGrid(model, viewState);

			break;


		case BUTTON_4:

			clearObjectFromWorld(model->world, xPos, yPos);
			model->world->gameBoard[yPos][xPos] = WALL_SQUARE;
			updateGrid(model, viewState);


			break;
		case BUTTON_5:

			clearObjectFromWorld(model->world, xPos, yPos);
			updateGrid(model, viewState);


			break;
		
		
		case SIDE_NUM_BUTTONS: // save button
			
			gameValidity = isWorldValid(model->world);
			model->world->gameValidity = gameValidity;
			if (gameValidity == VALID) {
				stateid = SAVE_GAME;
			}
			else {// 
				stateid = INVALID_WORLD;
			}


			break;

		case SIDE_NUM_BUTTONS+1: // go to main menu

			stateid = MAIN_MENU;

			break;


		case SIDE_NUM_BUTTONS + 2: //quit button


			stateid = QUIT;

			break;


		default: // if not clicked on a specific button, clicked on a grid button

			stateid = model->stateIdModel;
			
			markButtonStart(model, selectedButton->id, model->markedButton, viewState);

			break;

		}
		break;

	case MOVE_PLAYER_TO_DIRECTION:

		stateid = model->stateIdModel;
		direction = logicalEvent->direction;
		markedButtonByDirection(model, direction, viewState);
	

		break;

	default:
		stateid = model->stateIdModel;
		return stateid;
		break;
	}


	drawWidget(viewState);
	

	return stateid;



}


void* worldBuilderStop(GUI* gui) {
	freeTree(gui->viewState);
	return gui->model;
}

