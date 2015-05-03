#include "ChooseMenu.h"
#include <SDL.h>
#include "Model.h"
#include "Widget.h"
#include "Button.h"



LogicEvent* ChooseMenuTranslateEvent(Widget* viewState, SDL_Event* event, Model* model, int numButtons) {
	Widget* widget;

	switch (event->type) {
	case (SDL_QUIT) :
		return createLogicEvent(QUIT_EVENT, 0, 0, 0, 0, 0);
		break;
	case (SDL_KEYUP) :
		if (event->key.keysym.sym == SDLK_TAB) {
			return createLogicEvent(MARK_BUTTON, model->markedButton, (model->markedButton + 1) % numButtons, 0, 0, 0);
		}

					 if (event->key.keysym.sym == SDLK_RETURN) {
						 return createLogicEvent(SELECT_BUTTON, model->markedButton, model->markedButton, 0, 0, 0);
					 }
					 return createLogicEvent(NO_EVENT, 0, 0, 0, 0, 0);
					 break;
	case (SDL_MOUSEBUTTONUP) :
		widget = getWidgetFromPos(event->button.x, event->button.y, viewState);
		if (widget->type == BUTTON) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, widget->id, 0, 0, 0);
		}
		else {
			return createLogicEvent(NO_EVENT, 0, 0, 0, 0, 0);
		}
		break;
	default:
		return createLogicEvent(NO_EVENT, 0, 0, 0, 0, 0);
		break;
	}

}

//TODO: maybe move to another file
void markButton(Model* model, LogicEvent* logicalEvent, Widget* exMarkedButton, Widget* viewState, Widget* newMarkedButton) {
	model->markedButton = logicalEvent->buttonId;
	exMarkedButton = getWidgetFromId(logicalEvent->markedId, viewState);
	exMarkedButton->state = REGULAR;
	changeImage(exMarkedButton, REGULAR, exMarkedButton->imageFile);
	newMarkedButton = getWidgetFromId(logicalEvent->buttonId, viewState);
	newMarkedButton->state = MARKED;
	changeImage(newMarkedButton, MARKED, newMarkedButton->imageFile);
}

//TODO: maybe move to another file
void markButtonStart(Model* model, int buttonId, int markedId, Widget* viewState) {
	Widget* exMarkedButton = NULL;
	Widget* newMarkedButton = NULL;

	model->markedButton = buttonId;
	exMarkedButton = getWidgetFromId(markedId, viewState);
	exMarkedButton->state = REGULAR;
	changeImage(exMarkedButton, REGULAR, exMarkedButton->imageFile);
	newMarkedButton = getWidgetFromId(buttonId, viewState);
	newMarkedButton->state = MARKED;
	changeImage(newMarkedButton, MARKED, newMarkedButton->imageFile);
}
