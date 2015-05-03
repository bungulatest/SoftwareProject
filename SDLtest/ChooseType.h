#ifndef ChooseType_h__
#define ChooseType_h__

#include <SDL.h>
#include <SDL_video.h>
#include "gui.h"

#define CHOOSE_CAT_TITLE "Choose Your Cat:"
#define CHOOSE_MOUSE_TITLE "Choose Your Mouse:"
#define HUMAN_TEXT "Human"
#define MACHINE_TEXT "Machine"
#define BACK_TEXT "Back"
#define CHOOSE_TYPE_NUM_BUTTONS 3

typedef enum {
	BUTTON_HUMAN,
	BUTTON_MACHINE,
	BUTTON_BACK
} ChooseTypeButtons;



void createGUIChooseType(GUI* gui);

Widget* initializeChooseTypeWindow(SDL_Surface* windowSurface);
void chooseTypeStart(GUI* gui, Model* initData, SDL_Surface* windowSurface);
LogicEvent* chooseTypeTranslateEvent(Widget* viewState, SDL_Event* event, Model* model);
StateId chooseTypeHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent);
void* chooseTypeStop(GUI* gui, StateId nextStateId);

#endif // ChooseType_h__
