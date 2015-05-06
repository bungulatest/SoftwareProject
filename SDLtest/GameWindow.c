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
				buttonGrid[index] = createButton(squareRect, NULL, surface, REGULAR, "empty_square.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 0, 0, MAX_NUM_BUTTONS + index);
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
					buttonGrid[index] = createButton(squareRect, NULL, surface, REGULAR, "empty_square.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 0, 0, MAX_NUM_BUTTONS + index);
				}
				else {
					buttonGrid[index] = createButton(squareRect, NULL, surface, REGULAR, "wall_square.bmp", SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B), 0, 0, MAX_NUM_BUTTONS + index);
				}
				addChild(gridPanel, buttonGrid[index]);
				
			}
		}
	}

	return gridPanel;
}


