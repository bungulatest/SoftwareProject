#include "PlayGame.h"
#include "../ViewInfrastructure/Widget.h"
#include "../ViewInfrastructure/Window.h"
#include "../ViewInfrastructure/Label.h"
#include "../ViewInfrastructure/Button.h"
#include "../ViewInfrastructure/Panel.h"
#include "ChooseMenu.h"
#include "../ModelInfrastructure/Model.h"
#include "GameWindow.h"
#include "../Services/WorldFileService.h"
#include "../Services/EvaluationService.h"
#include "../Services/MoveService.h"
#include "../ModelInfrastructure/gui.h"

Bitmapfont* bitmapfont1;
Bitmapfont* bitmapfont2;
Bitmapfont* bitmapfont3;
GUI* guis[STATES_COUNT];

void createGUIPlayGame(GUI* gui) {
	gui->start = playGameStart;
	gui->viewTranslateEvent = playGameTranslateEvent;
	gui->presenterHandleEvent = playGameHandleEvent;
	gui->stop = playGameStop;
	gui->model = NULL;
	gui->viewState = NULL;
}


Widget* initializePlayGameWindow(SDL_Surface* windowSurface) {
	Widget* window = createWindow(windowSurface);

	Widget* gridPanel = createEmptyGrid(windowSurface);
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

	SDL_Rect pauseRect = { 250, TOP_PANEL_HEIGHT - 70, 10, 10 };
	Widget* pauseButton = createButton(pauseRect, NULL, windowSurface, REGULAR, "pause_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 10, 10, SIDE_NUM_BUTTONS, bitmapfont2);
	addChild(topPanel, pauseButton);

	SDL_Rect firstLabelRect = { 320, 20, 10, 10 };
	Widget* firstLabel = createLabel(firstLabelRect, NULL, windowSurface, 10, 10, bitmapfont1, NULL);
	addChild(topPanel, firstLabel);

	SDL_Rect secondLabelRect = { 300, 80, 10, 10 };
	Widget* secondLabel = createLabel(secondLabelRect, NULL, windowSurface, 10, 10, bitmapfont1, NULL);
	addChild(topPanel, secondLabel);

	SDL_Rect gameOverRect = { 200, 50, 10, 10 };
	Widget* gameOverLabel = createLabel(gameOverRect, NULL, windowSurface, 10, 10, bitmapfont3, NULL);
	gameOverLabel->isVisible = 0;
	addChild(topPanel, gameOverLabel);

	return window;
}

int isCurrPlayerHuman(Model* model) {
	if (model->world->currAnimal == CAT && model->gameConfig->catSkill == 0) {
		return 1;
	}
	else if (model->world->currAnimal == MOUSE && model->gameConfig->mouseSkill == 0) {
		return 1;
	}
	else {
		return 0;
	}
}



void updateTopPanel(Model* model, Widget* viewState) {
	char stringBuffer[STRING_MAX_LENGTH];
	char digitBuffer[4];
	if (model->world->currAnimal == CAT) {
		strcpy(stringBuffer, CAT_MOVE_TEXT);
	}
	else {
		strcpy(stringBuffer, MOUSE_MOVE_TEXT);
	}

	_itoa((model->world->totalTurns - model->world->currTurn), digitBuffer, 10);
	strcat(stringBuffer, digitBuffer);
	strcat(stringBuffer, ")");

	Widget* firstLabel = getWidgetFromPos(325, 25, viewState);
	setText(firstLabel, stringBuffer);

	Widget* secondLabel = getWidgetFromPos(305, 85, viewState);

	if (isCurrPlayerHuman(model) && !model->world->isPaused) {
		setText(secondLabel, HUMAN_WAITING_TEXT);
	}
	else if (isCurrPlayerHuman(model) && model->world->isPaused) {
		setText(secondLabel, HUMAN_PAUSED_TEXT);
	}
	else if (!isCurrPlayerHuman(model) && !model->world->isPaused) {
		setText(secondLabel, MACHINE_COMPUTING_TEXT);
	}
	else if (!isCurrPlayerHuman(model) && model->world->isPaused) {
		setText(secondLabel, MACHINE_PAUSED_TEXT);
	}

	Widget* pauseButton = getWidgetFromPos(255, TOP_PANEL_HEIGHT - 65, viewState);
	if (model->world->isPaused) {
		setText(pauseButton, RESUME_TEXT);
	}
	else {
		if (isCurrPlayerHuman(model)) {
			setText(pauseButton, PAUSE_TEXT);
		}
		else {
			setText(pauseButton, PAUSE_BEFORE_TEXT);
		}
	}

	Widget* gameOverLabel = getWidgetFromPos(205, 55, viewState);
	firstLabel->isVisible = 1;
	secondLabel->isVisible = 1;
	pauseButton->isVisible = 1;
	gameOverLabel->isVisible = 0;

	if (model->world->isGameOver) {
		firstLabel->isVisible = 0;
		secondLabel->isVisible = 0;
		pauseButton->isVisible = 0;
		gameOverLabel->isVisible = 1;

		if (model->world->gameStatus == CAT_WON) {
			setText(gameOverLabel, GAME_OVER_CAT_TEXT);
		}
		else if (model->world->gameStatus == MOUSE_WON) {
			setText(gameOverLabel, GAME_OVER_MOUSE_TEXT);
		}
		else if (model->world->gameStatus == DRAW) {
			setText(gameOverLabel, GAME_OVER_DRAW_TEXT);
		}

	}
}

void updateSidePanel(Model* model, Widget* viewState) {
	Widget* button1 = getWidgetFromPos(20, TOP_PANEL_HEIGHT + 65, viewState);
	Widget* button2 = getWidgetFromPos(20, TOP_PANEL_HEIGHT + 175, viewState);
	Widget* button3 = getWidgetFromPos(20, TOP_PANEL_HEIGHT + 285, viewState);
	Widget* button4 = getWidgetFromPos(20, TOP_PANEL_HEIGHT + 395, viewState);
	Widget* button5 = getWidgetFromPos(20, TOP_PANEL_HEIGHT + 505, viewState);
	Widget* sidePanel = getWidgetFromPos(20, TOP_PANEL_HEIGHT + 5, viewState);

	if (!model->world->isPaused && !model->world->isGameOver) {
		changeImage(button1, DISABLED, button1->imageFile);
		button1->state = DISABLED;
		changeImage(button2, DISABLED, button2->imageFile);
		button1->state = DISABLED;
		changeImage(button3, DISABLED, button3->imageFile);
		button1->state = DISABLED;
		changeImage(button4, DISABLED, button4->imageFile);
		button1->state = DISABLED;
		changeImage(button5, DISABLED, button5->imageFile);
		button1->state = DISABLED;

		sidePanel->backgroundColor = SDL_MapRGB(pixel_format, R_COLOR_DISABLED, G_COLOR_DISABLED, B_COLOR_DISABLED);
	}
	else {
		changeImage(button1, REGULAR, button1->imageFile);
		button1->state = REGULAR;
		changeImage(button2, REGULAR, button2->imageFile);
		button1->state = REGULAR;
		changeImage(button3, REGULAR, button3->imageFile);
		button1->state = REGULAR;
		changeImage(button4, REGULAR, button4->imageFile);
		button1->state = REGULAR;
		changeImage(button5, REGULAR, button5->imageFile);
		button1->state = REGULAR;

		sidePanel->backgroundColor = SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B);
	}
}



void updateView(Model* model, Widget* viewState) {


	updateTopPanel(model, viewState);
	updateSidePanel(model, viewState);
	updateGrid(model, viewState);
}

void makeMachineMove(Model* model, Widget* viewState) {
	Direction bestDirection;
	SDL_Delay(MACHINE_DELAY);
	bestDirection = getBestMove(model->world, model->gameConfig->catSkill, model->gameConfig->mouseSkill);

	makeMove(bestDirection, model->world);
	updateView(model, viewState);
	drawWidget(viewState);


	return;
}


void playGameStart(GUI* gui, Model* initData, SDL_Surface* windowSurface) {
	gui->viewState = initializePlayGameWindow(windowSurface);

	gui->model = guis[gui->stateId]->model;
	if (initData->world != NULL) { // coming from "reconfigure" path
		gui->model->gameConfig = initData->gameConfig;
		gui->model->world = initData->world;
	}
	else { // we haven't creates "world" struct yet
		World* world = createWorld(initData->gameConfig->worldIndex);
		gui->model = createModel(gui->stateId, initData, 0);
		gui->model->gameConfig = initData->gameConfig;
		gui->model->world = world;
	}
	updateGameStatus(gui->model->world);
	updateView(gui->model, gui->viewState);



	drawWidget(gui->viewState);

}


LogicEvent* playGameTranslateEvent(Widget* viewState, SDL_Event* event, Model* model) {
	if (event->type == SDL_MOUSEBUTTONUP) {
		return ChooseMenuTranslateEvent(viewState, event, model, PLAY_GAME_NUM_BUTTONS);
	}

	if (event->type == SDL_KEYUP) {

		// side panel buttons
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

		// pause button
		else if (event->key.keysym.sym == SDLK_SPACE) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, SIDE_NUM_BUTTONS, 0, 0, 0);
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


StateId playGameHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent) {
	Widget* exMarkedButton = NULL;
	Widget* newMarkedButton = NULL;
	Widget* selectedButton = NULL;
	StateId stateid = model->stateIdModel;
	int xPos, yPos;
	Direction direction = -1;
	int moveLegal = 0;

	switch (logicalEvent->type) {

	case SELECT_BUTTON:
		selectedButton = getWidgetFromId(logicalEvent->buttonId, viewState);

		// return the appropriate StateId
		switch (selectedButton->id) {
		case BUTTON_1:
			if (model->world->isPaused == 1 || model->world->isGameOver) {
				if (model->gameConfig->mouseSkill == 0) { // mouse is human
					stateid = CHOOSE_MOUSE;
				}
				else {
					stateid = MOUSE_CHOOSE_SKILL;
				}
			}

			break;
		case BUTTON_2:
			if (model->world->isPaused == 1 || model->world->isGameOver) {
				if (model->gameConfig->catSkill == 0) { // cat is human
					stateid = CHOOSE_CAT;
				}
				else {
					stateid = CAT_CHOOSE_SKILL;
				}
			}



			break;

		case BUTTON_3:

			if (model->world->isPaused == 1 || model->world->isGameOver) {
				freeWorld(model->world);
				model->world = createWorld(model->gameConfig->worldIndex); // rebuilt the "world" struct without changing configuration

				model->world->isPaused = 0;

				updateView(model, viewState);
				drawWidget(viewState);
				if (!isCurrPlayerHuman(model)) {
					makeMachineMove(model, viewState);
				}
				stateid = model->stateIdModel;
			}

			break;

		case BUTTON_4:

			if (model->world->isPaused == 1 || model->world->isGameOver) {
				stateid = MAIN_MENU;
			}

			break;
		case BUTTON_5:

			if (model->world->isPaused == 1 || model->world->isGameOver) {
				stateid = QUIT;
			}

			break;
		case SIDE_NUM_BUTTONS: // pause button
			stateid = model->stateIdModel;
			if (model->world->isPaused == 1) {
				model->world->isPaused = 0;
			}
			else {
				model->world->isPaused = 1;
			}

			updateView(model, viewState);
			break;

		default: // if not clicked on a specific button, clicked on a grid button
			stateid = model->stateIdModel;
			xPos = (selectedButton->id - MAX_NUM_BUTTONS) % BOARD_SIZE;
			yPos = ((selectedButton->id - MAX_NUM_BUTTONS) - xPos) / BOARD_SIZE;
			direction = getDirectionFromPos(xPos, yPos, model);
			moveLegal = isMoveLegal(direction, model->world);

			break;
		}
		break;

	case MOVE_PLAYER_TO_DIRECTION:
		stateid = model->stateIdModel;
		direction = logicalEvent->direction;
		moveLegal = isMoveLegal(direction, model->world);

		break;
	case NO_EVENT:
		stateid = model->stateIdModel;

		if (!isCurrPlayerHuman(model) && !model->world->isPaused && !model->world->isGameOver) {
			makeMachineMove(model, viewState);
		}

		return stateid;
		break;
	default:
		stateid = model->stateIdModel;
		return stateid;
		break;
	}

	if (moveLegal && !model->world->isPaused && !model->world->isGameOver) {

		makeMove(direction, model->world);
		updateView(model, viewState);
		drawWidget(viewState);
		if (!isCurrPlayerHuman(model) && !model->world->isPaused && !model->world->isGameOver) {
			makeMachineMove(model, viewState);
		}
	}
	else {
		drawWidget(viewState);
	}

	return stateid;
}

void* playGameStop(GUI* gui) {
	freeTree(gui->viewState);
	return gui->model;
}