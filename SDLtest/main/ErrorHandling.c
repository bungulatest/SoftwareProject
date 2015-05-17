#include "ErrorHandling.h"
#include "../ModelInfrastructure/gui.h"
#include "../ModelInfrastructure/Model.h"
#include "../ViewInfrastructure/Widget.h"

#define NUM_STATES 12

GUI* guis[NUM_STATES];
Bitmapfont* bitmapfont1;
Bitmapfont* bitmapfont2;
Bitmapfont* bitmapfont3;

void freeGui(GUI* gui) {
	if (gui != NULL) {
		if (gui->viewState != NULL) {
			if (!gui->viewState->isFree) {
				freeTree(gui->viewState);
			}
			freeWidget(gui->viewState);
		}
		if (gui->model != NULL && !gui->isModelFree) {
			freeModel(gui);
		}
		
	}
	
}

void freeModel(GUI* gui) {
	if (gui->model != NULL && !gui->isModelFree) {
		
		if (gui->model->gameConfig != NULL && !gui->isGameConfigFree) {
			free(gui->model->gameConfig);
			guis[CHOOSE_CAT]->isGameConfigFree = 1;
			guis[CHOOSE_MOUSE]->isGameConfigFree = 1;
			guis[CAT_CHOOSE_SKILL]->isGameConfigFree = 1;
			guis[MOUSE_CHOOSE_SKILL]->isGameConfigFree = 1;
			guis[PLAY_GAME]->isGameConfigFree = 1;
		}
		if (gui->model->world != NULL && !gui->isWorldFree) {
			freeWorld(gui->model->world);
			guis[CHOOSE_CAT]->isWorldFree = 1;
			guis[CHOOSE_MOUSE]->isWorldFree = 1;
			guis[CAT_CHOOSE_SKILL]->isWorldFree = 1;
			guis[MOUSE_CHOOSE_SKILL]->isWorldFree = 1;
			guis[EDIT_GAME]->isWorldFree = 1;
			guis[SAVE_GAME]->isWorldFree = 1;
			guis[PLAY_GAME]->isWorldFree = 1;
			guis[NEW_WORLD_BUILDER]->isWorldFree = 1;
			guis[EDIT_WORLD_BUILDER]->isWorldFree = 1;
			guis[INVALID_WORLD]->isWorldFree = 1;
		}

		free(gui->model);
		gui->isModelFree = 1;
		
	}


}

void freeGuiArray() {
	int i;
	for (i = 0; i < NUM_STATES; i++) {
		freeGui(guis[i]);
	}
}

void exitProgram(void) {
	freeGuiArray();
	if (bitmapfont1 != NULL) {
		destroyFont(bitmapfont1);
	}
	if (bitmapfont2 != NULL) {
		destroyFont(bitmapfont2);
	}
	if (bitmapfont3 != NULL) {
		destroyFont(bitmapfont3);
	}
	SDL_Quit();
}