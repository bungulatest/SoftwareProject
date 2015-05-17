#include "GameWindow.h"
#include "../ModelInfrastructure/Model.h"
#include "../ViewInfrastructure/Button.h"
#include "../ViewInfrastructure/Widget.h"
#include "../ViewInfrastructure/Label.h"
#include "../ViewInfrastructure/Panel.h"
#include "../ViewInfrastructure/Window.h"
#include "../ViewInfrastructure/BitmapFont.h"
#include "../Services/WorldFileService.h"
#include "../Services/MoveService.h"
#include <SDL.h>
#include <SDL_video.h>

Bitmapfont* bitmapfont2;

Widget* createEmptyGrid(SDL_Surface* surface) {
	SDL_Rect gridRect = { SIDE_PANEL_WIDTH, TOP_PANEL_HEIGHT, GRID_SIZE, GRID_SIZE };
	Widget* gridPanel = createPanel(surface, gridRect, SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B));

	int i, j, index;
	Widget* buttonGrid[BOARD_SIZE*BOARD_SIZE];

	SDL_Rect squareRect = { 0, 0, GRID_SIZE, GRID_SIZE };
	
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				index = i*BOARD_SIZE + j;
				squareRect.y = i*(SQUARE_SIZE - EDGE_LENGTH);
				squareRect.x = j*(SQUARE_SIZE - EDGE_LENGTH);
				buttonGrid[index] = createButton(squareRect, NULL, surface, REGULAR, "empty_square.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 0, 0, MAX_NUM_BUTTONS + index, NULL);
				addChild(gridPanel, buttonGrid[index]);
			}
		}
	
	

	return gridPanel;
}

void updateGrid(Model* model, Widget* viewState) {
	int i, j, index;
	int catIndex, mouseIndex, cheeseIndex;
	Widget* currButton;
	Widget* catButton;
	Widget* mouseButton;
	Widget* cheeseButton;
	char currSquare;

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			index = i*BOARD_SIZE + j;
			currSquare = model->world->gameBoard[i][j];
			currButton = getWidgetFromId(MAX_NUM_BUTTONS + index, viewState);

			if (currSquare == EMPTY_SQUARE) {
				changeImage(currButton, currButton->state, "empty_square.bmp");
			}
			else if (currSquare == WALL_SQUARE) {
				changeImage(currButton, currButton->state, "wall_square.bmp");
			}
		}
	}
	//only display the objuects if they exist
	if (model->world->catXPos != -1) {
		catIndex = model->world->catYPos*BOARD_SIZE + model->world->catXPos;
		catButton = getWidgetFromId(MAX_NUM_BUTTONS + catIndex, viewState);
		changeImage(catButton, catButton->state, "cat.bmp");
	}

	if (model->world->mouseXPos != -1) {
		mouseIndex = model->world->mouseYPos*BOARD_SIZE + model->world->mouseXPos;
		mouseButton = getWidgetFromId(MAX_NUM_BUTTONS + mouseIndex, viewState);
		changeImage(mouseButton, mouseButton->state, "mouse.bmp");
	}
	if (model->world->cheeseXPos != -1) {
		cheeseIndex = model->world->cheeseYPos*BOARD_SIZE + model->world->cheeseXPos;
		cheeseButton = getWidgetFromId(MAX_NUM_BUTTONS + cheeseIndex, viewState);
		changeImage(cheeseButton, cheeseButton->state, "cheese.bmp");
	}

}


Widget* createSidePanel(SDL_Surface* surface) {
	SDL_Rect sideRect = { 0, TOP_PANEL_HEIGHT, SIDE_PANEL_WIDTH, SIDE_PANEL_HEIGHT };
	Widget* sidePanel = createPanel(surface, sideRect, SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B));

	SDL_Rect borderLineRect = { 0, 0, 10, 10 };
	Widget* borderLineLabel = createLabel(borderLineRect, NULL, surface, 0, 0, NULL, "side_panel_border.bmp");
	addChild(sidePanel, borderLineLabel);


	SDL_Rect button1Rect = { 18, 60, 200, 50 };
	SDL_Rect button2Rect = { 18, 170, 200, 50 };
	SDL_Rect button3Rect = { 18, 280, 200, 50 };
	SDL_Rect button4Rect = { 18, 390, 200, 50 };
	SDL_Rect button5Rect = { 18, 500, 200, 50 };

	Widget* button1 = createButton(button1Rect, NULL, surface, REGULAR, "side_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 10, 10, BUTTON_1, bitmapfont2);
	addChild(sidePanel, button1);

	Widget* button2 = createButton(button2Rect, NULL, surface, REGULAR, "side_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 10, 10, BUTTON_2, bitmapfont2);
	addChild(sidePanel, button2);

	Widget* button3 = createButton(button3Rect, NULL, surface, REGULAR, "side_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 10, 10, BUTTON_3, bitmapfont2);
	addChild(sidePanel, button3);

	Widget* button4 = createButton(button4Rect, NULL, surface, REGULAR, "side_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 10, 10, BUTTON_4, bitmapfont2);
	addChild(sidePanel, button4);

	Widget* button5 = createButton(button5Rect, NULL, surface, REGULAR, "side_button.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 10, 10, BUTTON_5, bitmapfont2);
	addChild(sidePanel, button5);

	return sidePanel;
}

Widget* createTopPanel(SDL_Surface* surface) {
	SDL_Rect topRect = { 0, 0, WIN_W, TOP_PANEL_HEIGHT };
	Widget* topPanel = createPanel(surface, topRect, SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B));

	return topPanel;
}