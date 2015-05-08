#ifndef PlayGame_h__
#define PlayGame_h__
#include "Model.h"
#include "gui.h"

#include <SDL.h>
#include <SDL_video.h>

#define REC_MOUSE_TEXT "Reconfigure Mouse\n\n        (F1)"
#define REC_CAT_TEXT "Reconfigure Cat\n\n        (F2)"
#define RESTART_GAME_TEXT "Restart Game (F3)"
#define GO_TO_MAIN_MENU_TEXT "Go to Main Menu\n\n        (F4)"
#define QUIT_PROGRAM_TEXT "Quit Program (Esc)"
#define PAUSE_BEFORE_TEXT "Pause Before Next Move (Space)"
#define PAUSE_TEXT "Pasue (Space)"
#define RESUME_TEXT "Resume Game (Space)"
#define GAME_OVER_CAT_TEXT "Game Over - Cat Wins!!!"
#define GAME_OVER_MOUSE_TEXT "Game Over - Mouse Wins!!!"
#define GAME_OVER_DRAW_TEXT "Game Over - Timeout!!!"
#define CAT_MOVE_TEXT "Cat's move "
#define MOUSE_MOVE_TEXT "Mouse's move "
#define HUMAN_WAITING_TEXT "Human - waiting for next move..."
#define HUMAN_PAUSED_TEXT "Human - paused"
#define MACHINE_COMPUTING_TEXT "Machine - computing..."
#define MACHINE_PAUSED_TEXT "Machine - paused"

#define PLAY_GAME_NUM_BUTTONS 6


void createGUIPlayGame(GUI* gui);

Widget* initializePlayGameWindow(SDL_Surface* windowSurface);
void playGameStart(GUI* gui, Model* initData, SDL_Surface* windowSurface);
LogicEvent* playGameTranslateEvent(Widget* viewState, SDL_Event* event, Model* model);
StateId playGameHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent);
void* playGameStop(GUI* gui, StateId nextStateId);
#endif // PlayGame_h__