#ifndef GameWindow_h__
#define GameWindow_h__

// generic functions for building gui of both game play and world builder

#include "../ViewInfrastructure/Widget.h"
#include "../ModelInfrastructure/Model.h"
#include <SDL.h>
#include <SDL_video.h>

#define EDGE_LENGTH 3
#define SQUARE_SIZE 86
#define GRID_SIZE (((SQUARE_SIZE)*(BOARD_SIZE)) - (((BOARD_SIZE)-1)*(EDGE_LENGTH)))
#define SIDE_PANEL_HEIGHT GRID_SIZE
#define SIDE_PANEL_WIDTH ((WIN_W) - (GRID_SIZE))
#define TOP_PANEL_HEIGHT ((WIN_W) - (GRID_SIZE))
#define TOP_PANEL_WIDTH WIN_W

// maximum size of string buffer for using strcat
#define STRING_MAX_LENGTH 40

// maximum number of non grid buttons (to avoid collisions in buttonid)
#define MAX_NUM_BUTTONS 8

#define SIDE_NUM_BUTTONS 5



Widget* createEmptyGrid(SDL_Surface* surface);
void updateGrid(Model* model, Widget* viewState);
Widget* createSidePanel(SDL_Surface* surface);
Widget* createTopPanel(SDL_Surface* surface);
#endif // GameWindow_h__
