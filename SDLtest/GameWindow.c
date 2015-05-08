#include "GameWindow.h"
#include "Model.h"
#include "Button.h"
#include "Widget.h"
#include "Label.h"
#include "Panel.h"
#include "Window.h"
#include "BitmapFont.h"
#include <SDL.h>
#include <SDL_video.h>

Bitmapfont* bitmapfont2;

Widget* createGridFromModel(Model* model, SDL_Surface* surface) {
	SDL_Rect gridRect = { SIDE_PANEL_WIDTH, TOP_PANEL_HEIGHT, GRID_SIZE, GRID_SIZE };
	Widget* gridPanel = createPanel(surface, gridRect, SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B));

	int i, j, index;
	//Widget** buttonGrid = (Widget**)malloc(sizeof(Widget*) * BOARD_SIZE*BOARD_SIZE); //TODO: free buttonGrid array on the stop function of the GUI (after calling freeTree())
	Widget* buttonGrid[BOARD_SIZE*BOARD_SIZE];

	SDL_Rect squareRect = { 0, 0, GRID_SIZE, GRID_SIZE };
	if (model == NULL || model->world == NULL || model->world->gameBoard == NULL) {
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				index = i*BOARD_SIZE + j;
				squareRect.x = 0 + i*(SQUARE_SIZE - EDGE_LENGTH);
				squareRect.y = 0 + j*(SQUARE_SIZE - EDGE_LENGTH);
				buttonGrid[index] = createButton(squareRect, NULL, surface, REGULAR, "empty_square.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 0, 0, MAX_NUM_BUTTONS + index, NULL);
				addChild(gridPanel, buttonGrid[index]);
			}
		}
	}
	else {
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				index = i*BOARD_SIZE + j;
				squareRect.x = SIDE_PANEL_WIDTH + i*(SQUARE_SIZE - EDGE_LENGTH);
				squareRect.y = TOP_PANEL_HEIGHT + j*(SQUARE_SIZE - EDGE_LENGTH);
				if (model->world->gameBoard[i][j] != WALL) {
					buttonGrid[index] = createButton(squareRect, NULL, surface, REGULAR, "empty_square.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 0, 0, MAX_NUM_BUTTONS + index, NULL);
				}
				else {
					buttonGrid[index] = createButton(squareRect, NULL, surface, REGULAR, "wall_square.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 0, 0, MAX_NUM_BUTTONS + index, NULL);
				}
				addChild(gridPanel, buttonGrid[index]);
				
			}
		}
	}

	return gridPanel;
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