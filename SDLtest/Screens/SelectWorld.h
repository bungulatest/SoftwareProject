#ifndef SelectWorld_h__
#define SelectWorld_h__

#include <SDL.h>
#include <SDL_video.h>
#include "../ModelInfrastructure/gui.h"

#define LOAD_GAME_TITLE "Load Game"
#define SAVE_GAME_TITLE "Save Game"
#define EDIT_GAME_TITLE "Edit Game"
#define SELECT_WORLD_COOSE_TEXT "World "
#define SELECT_WORLD_DONE_TEXT "Done"
#define SELECT_WORLD_BACK_TEXT "Back"
#define SELECT_WORLD_NUM_BUTTONS 3
#define DEFAULT_WORLD 1
#define MAX_WORLD 5
#define MIN_WORLD 1
#define SELECT_WORLD_STRING_LENGTH 8


void createGuiSelectWorld(GUI* gui);
Widget* initializeChooseWorldWindow(SDL_Surface* windowSurface);
void changeWorldButton(Widget* viewState);
void selectWorldStart(GUI* gui, Model* initData, SDL_Surface* windowSurface);
LogicEvent* selectWorldTranslateUpOrDownEvent(Widget* viewState, SDL_Event* event, Model* model);
LogicEvent* selectWorldTranslateEvent(Widget* viewState, SDL_Event* event, Model* model);
StateId selectWorldHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent);
void* selectWorldStop(GUI* gui);
#endif // SelectWorld_h__
