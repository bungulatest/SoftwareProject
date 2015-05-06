#ifndef PlayGame_h__
#define PlayGame_h__
#include "Model.h"
#include "gui.h"

#include <SDL.h>
#include <SDL_video.h>

void createGUIPlayGame(GUI* gui);

Widget* initializePlayGameWindow(SDL_Surface* windowSurface);
void playGameStart(GUI* gui, Model* initData, SDL_Surface* windowSurface);
LogicEvent* playGameTranslateEvent(Widget* viewState, SDL_Event* event, Model* model);
StateId playGameHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent);
void* playGameStop(GUI* gui, StateId nextStateId);
#endif // PlayGame_h__
