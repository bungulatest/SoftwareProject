#ifndef WorldBuilder_h__
#define WorldBuilder_h__

#include "Model.h"
#include "gui.h"

#include <SDL.h>
#include <SDL_video.h>

#define NEW_WORLD_TITLE "New World"
#define EDIT_WORLD_TITLE "World "
#define SAVE_WORLD_TEXT "Save World (S)"
#define GO_TO_MAIN_MENU_TEXT_F1 "Go to Main Menu (F1)"
#define QUIT_PROGRAM_TEXT_ESC "Quit Program (Esc)"
#define PLACE_MOUSE_TEXT "   Place Mouse (M)"
#define PLACE_CAT_TEXT "   Place Cat (C)"
#define PLACE_CHEESE_TEXT "   Place Cheese (P)"
#define PLACE_WALL_TEXT "   Place Wall (W)"
#define PLACE_EMPTY_SPACE_TEXT " Place Empty Space\n\n      (Space)"
#define WORLD_BUILDER_NUM_BUTTONS 8


void createGUIWorldBuilder(GUI* gui);
Widget* initializeWorldBuilderWindow(SDL_Surface* windowSurface);
void worldBuilderStart(GUI* gui, Model* initData, SDL_Surface* windowSurface);
LogicEvent* worldBuilderTranslateEvent(Widget* viewState, SDL_Event* event, Model* model);
void markedButtonByDirection(Model* model, Direction direction, Widget* viewState);
void clearObjectFromWorld(World* world, int xPos, int yPos);
StateId worldBuilderHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent);
void* worldBuilderStop(GUI* gui);
#endif // WorldBuilder_h__
