#ifndef gui_h__
#define gui_h__

#include <SDL.h>
#include <SDL_video.h>
#include "Widget.h"
#include "Model.h"

typedef enum {
	MAIN_MENU,
	CHOOSE_CAT,
	CHOOSE_MOUSE,
	CAT_CHOOSE_SKILL,
	MOUSE_CHOOSE_SKILL,
	LOAD_GAME,
	EDIT_GAME,
	SAVE_GAME,
	PLAY_GAME,
	NEW_WORLD_BUILDER, // after clicking "new game"
	EDIT_WORLD_BUILDER, // after clicking "edit game"
	
	/*...*/
	QUIT
} StateId;

// The GUI structure.
typedef struct GUI {
	// The unique state id:
	StateId stateId;

	// The model and viewState. Assumed to be NULL until GUI is started and once it is stopped.
	// The model and view state should NOT hold references to each other.
	Model* model;
	Widget* viewState;

	// The function pointer for starting this GUI - initializing the model and viewState as well as drawing the initial state of the GUI.
	void(*start) (struct GUI* gui, Model* initData, SDL_Surface* windowSurface);

	// The function pointer for translating the SDL_Event to a logical event according to the current viewState.
	// The logical event will be passed to the presenter which will take care of updating the model and/or the view.
	// The data type of the logical event is assumed to be know by the presenter and will also be freed by it, if need be.
	LogicEvent* (*viewTranslateEvent) (Widget* viewState, SDL_Event* event, Model* model);

	// The function pointer for handling the given logical event.
	// may or may not update the model and/or viewState.
	// Returns the next state to transition to.
	StateId(*presenterHandleEvent) (void* model, Widget* viewState, LogicEvent* logicalEvent);

	// The function pointer for deactivating this GUI - freeing the model and viewState and any other resources used.
	// Returns the initialization data for the next state (can be NULL if not required).
	void* (*stop) (struct GUI* gui);
} GUI;

#endif // gui_h__
