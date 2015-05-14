#ifndef ChooseMenu_h__
#define ChooseMenu_h__

#include <SDL.h>
#include <SDL_video.h>
#include "../ViewInfrastructure/Widget.h"
#include "../ModelInfrastructure/Model.h"

LogicEvent* ChooseMenuTranslateEvent(Widget* viewState, SDL_Event* event, Model* model, int numButtons);

void markButton(Model* model, LogicEvent* logicalEvent, Widget* exMarkedButton, Widget* viewState, Widget* newMarkedButton);
void markButtonStart(Model* model, int buttonId, int markedId, Widget* viewState);
#endif // ChooseMenu_h__
