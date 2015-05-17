#ifndef MainMenu_h__
#define MainMenu_h__

#include "../ModelInfrastructure/Model.h"
#include "../ModelInfrastructure/gui.h"

#include <SDL.h>
#include <SDL_video.h>

#define MAIN_TITLE "Cat & Mouse"
#define NEW_GAME_TEXT "New Game"
#define LOAD_GAME_TEXT "Load Game"
#define CREATE_GAME_TEXT "Create Game"
#define EDIT_GAME_TEXT "Edit Game"
#define QUIT_TEXT "Quit"
#define MAIN_NUM_BUTTONS 5

typedef enum {
	BUTTON_NEW_GAME,
	BUTTON_LOAD_GAME,
	BUTTON_CREATE_GAME,
	BUTTON_EDIT_GAME,
	BUTTON_QUIT
} MainButtons;

void createGUIMain(GUI* gui);



Widget* initializeMainWindow(SDL_Surface* windowSurface);
void MainStart(GUI* gui, Model* initData, SDL_Surface* windowSurface);
LogicEvent* MainTranslateEvent(Widget* viewState, SDL_Event* event, Model* model);
StateId MainHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent);
void* MainStop(GUI* gui);
#endif // MainMenu_h__