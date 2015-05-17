#ifndef PlayGame_h__
#define PlayGame_h__

#include "../ModelInfrastructure/Model.h"
#include "../ModelInfrastructure/gui.h"

#include <SDL.h>
#include <SDL_video.h>

#define REC_MOUSE_TEXT "Reconfigure Mouse\n\n        (F1)"
#define REC_CAT_TEXT "Reconfigure Cat\n\n        (F2)"
#define RESTART_GAME_TEXT "Restart Game (F3)"
#define GO_TO_MAIN_MENU_TEXT "Go to Main Menu\n\n        (F4)"
#define QUIT_PROGRAM_TEXT "Quit Program (Esc)"
#define PAUSE_BEFORE_TEXT "Pause Before Next Move (Space)"
#define PAUSE_TEXT "Pause (Space)"
#define RESUME_TEXT "Resume Game (Space)"
#define GAME_OVER_CAT_TEXT "Game Over - Cat Wins!!!"
#define GAME_OVER_MOUSE_TEXT "Game Over - Mouse Wins!!!"
#define GAME_OVER_DRAW_TEXT "Game Over - Timeout!!!"
#define CAT_MOVE_TEXT "Cat's move ("
#define MOUSE_MOVE_TEXT "Mouse's move ("
#define HUMAN_WAITING_TEXT "Human - waiting for next move..."
#define HUMAN_PAUSED_TEXT "Human - paused"
#define MACHINE_COMPUTING_TEXT "Machine - computing..."
#define MACHINE_PAUSED_TEXT "Machine - paused"



#define PLAY_GAME_NUM_BUTTONS 40 

#define R_COLOR_DISABLED 127
#define G_COLOR_DISABLED 127
#define B_COLOR_DISABLED 127

void createGUIPlayGame(GUI* gui);

Widget* initializePlayGameWindow(SDL_Surface* windowSurface);
int isCurrPlayerHuman(Model* model);
void updateTopPanel(Model* model, Widget* viewState);
void updateSidePanel(Model* model, Widget* viewState);
void updateView(Model* model, Widget* viewState);

void makeMachineMove(Model* model, Widget* viewState);
void playGameStart(GUI* gui, Model* initData, SDL_Surface* windowSurface);
LogicEvent* playGameTranslateEvent(Widget* viewState, SDL_Event* event, Model* model);
StateId playGameHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent);
void* playGameStop(GUI* gui);
#endif // PlayGame_h__
