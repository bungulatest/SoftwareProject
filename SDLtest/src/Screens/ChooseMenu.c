#include "ChooseMenu.h"
#include <SDL.h>
#include "../ModelInfrastructure/Model.h"
#include "../ViewInfrastructure/Widget.h"

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
					 return createLogicEvent(OTHER_EVENT, 0, 0, 0, 0, 0);
					 break;
	case (SDL_MOUSEBUTTONUP) :
		widget = getWidgetFromPos(event->button.x, event->button.y, viewState);
		if (widget->type == BUTTON) {
			return createLogicEvent(SELECT_BUTTON, model->markedButton, widget->id, 0, 0, 0);
		}
		else {
			return createLogicEvent(OTHER_EVENT, 0, 0, 0, 0, 0);
		}
		break;
	default:
		return createLogicEvent(OTHER_EVENT, 0, 0, 0, 0, 0);
		break;
	}

}



