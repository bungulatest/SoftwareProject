#ifndef InvalidWorld_h__
#define InvalidWorld_h__

#include "../ModelInfrastructure/Model.h"
#include "../ModelInfrastructure/gui.h"

#include <SDL.h>
#include <SDL_video.h>

#define CAT_IS_MISSING_TEXT "Cat is missing"
#define MOUSE_IS_MISSING_TEXT "Mouse is missing"
#define CHEESE_IS_MISSING_TEXT "Cheese is missing"
#define BACK_TEXT "Back"
#define INVALID_WORLD_TITLE_TEXT "Cannot save invalid world!"
#define INVALID_WORLD_NUM_BUTTONS 1

typedef enum {
	BACK_BUTTON
} InvalidWorldButtons;


void createGUIInvalidWorld(GUI* gui);
Widget* initializeInvalidWorldWindow(SDL_Surface* windowSurface);
void invalidWorldStart(GUI* gui, Model* initData, SDL_Surface* windowSurface);
LogicEvent* invalidWorldTranslateEvent(Widget* viewState, SDL_Event* event, Model* model);
StateId invalidWorldHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent);
void* invalidWorldStop(GUI* gui);

#endif // InvalidWorld_h__
