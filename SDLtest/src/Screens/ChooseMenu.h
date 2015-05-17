#ifndef ChooseMenu_h__
#define ChooseMenu_h__

#include <SDL.h>
#include <SDL_video.h>
#include "../ViewInfrastructure/Widget.h"
#include "../ModelInfrastructure/Model.h"

LogicEvent* ChooseMenuTranslateEvent(Widget* viewState, SDL_Event* event, Model* model, int numButtons);

#endif // ChooseMenu_h__
