#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>
#include "../ViewInfrastructure/Widget.h"
#include "../ModelInfrastructure/GuiFactory.h"
#include "../ModelInfrastructure/gui.h"
#include "ConsoleMode.h"




SDL_PixelFormat* pixel_format;
Bitmapfont* bitmapfont1;
Bitmapfont* bitmapfont2;
Bitmapfont* bitmapfont3;
GUI* guis[STATES_COUNT];

// Should be externalized in some header file.
int isError = 0;

int main(int argc, char* args[]) {

	// console mode input validation
	if (argc > 3) {
		printf("Error: too many arguments");
		exit(1);
	}
	else if (argc == 3) {
		if (!strcmp(args[1], "-console")) {
			if (!strcmp(args[2], "mouse")) {
				startConsole(MOUSE);
			}
			else if (!strcmp(args[2], "cat")) {
				startConsole(CAT);
			}
			else {
				printf("Error: animal argument is invalid");
				exit(1);
			}
		}
		else {
			printf("Error: first parameter invalid");
			exit(1);
		}
	}

	else if (argc == 2) {
		printf("Error: incorrect number of arguments");
		exit(1);
	}

	// if not in console mode, start graphical mode

	// Initialize SDL and make sure it quits
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "ERROR: unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	// Create window surface
	SDL_WM_SetCaption("Cat And Mouse", "Cat And Mouse");
	SDL_Surface *mainWindow = createSurface();
	pixel_format = mainWindow->format;


	// create global bitmap fonts
	SDL_Surface* fontSurface1 = SDL_LoadBMP(BITMAP_FONT_FILE1);
	if (fontSurface1 == NULL) {
		fprintf(stderr, "ERROR: failed to load bmp file: %s\n", SDL_GetError());
		exit(1);
	}
	bitmapfont1 = createFont(fontSurface1, SDL_MapRGB(pixel_format, BITMAPR, BITMAPG, BITMAPB));

	SDL_Surface* fontSurface2 = SDL_LoadBMP(BITMAP_FONT_FILE2);
	if (fontSurface2 == NULL) {
		fprintf(stderr, "ERROR: failed to load bmp file: %s\n", SDL_GetError());
		exit(1);
	}
	bitmapfont2 = createFont(fontSurface2, SDL_MapRGB(pixel_format, BITMAPR, BITMAPG, BITMAPB));

	SDL_Surface* fontSurface3 = SDL_LoadBMP(BITMAP_FONT_FILE3);
	if (fontSurface3 == NULL) {
		fprintf(stderr, "ERROR: failed to load bmp file: %s\n", SDL_GetError());
		exit(1);
	}
	bitmapfont3 = createFont(fontSurface3, SDL_MapRGB(pixel_format, BITMAPR, BITMAPG, BITMAPB));

	// initialize GUI structs mapping by state ids:
	guis[MAIN_MENU] = createGUIForState(MAIN_MENU);
	guis[CHOOSE_CAT] = createGUIForState(CHOOSE_CAT);
	guis[CHOOSE_MOUSE] = createGUIForState(CHOOSE_MOUSE);
	guis[CAT_CHOOSE_SKILL] = createGUIForState(CAT_CHOOSE_SKILL);
	guis[MOUSE_CHOOSE_SKILL] = createGUIForState(MOUSE_CHOOSE_SKILL);
	guis[LOAD_GAME] = createGUIForState(LOAD_GAME);
	guis[EDIT_GAME] = createGUIForState(EDIT_GAME);
	guis[SAVE_GAME] = createGUIForState(SAVE_GAME);
	guis[PLAY_GAME] = createGUIForState(PLAY_GAME);
	guis[NEW_WORLD_BUILDER] = createGUIForState(NEW_WORLD_BUILDER);
	guis[EDIT_WORLD_BUILDER] = createGUIForState(EDIT_WORLD_BUILDER);
	guis[INVALID_WORLD] = createGUIForState(INVALID_WORLD);
	
	

	// Starting the default/initial GUI:
	StateId nextStateId = MAIN_MENU;

	GUI* activeGUI = guis[nextStateId];
	activeGUI->start(activeGUI, NULL, mainWindow);

	int wait = 1;
	while (!isError && nextStateId != QUIT) {
		SDL_Event event;
		LogicEvent* logicalEvent;

		
		if (SDL_PollEvent(&event) != 0) {
			wait = 0;
			// translating the SDL event to a logical event using the view:
			logicalEvent = activeGUI->viewTranslateEvent(activeGUI->viewState, &event, activeGUI->model);
		}

		else { // create a NO_EVENT for machine-machine games if the event queue is empty
			wait = 1;
			logicalEvent = createLogicEvent(NO_EVENT, 0, 0, 0, 0, 0);
		}

			// Handling the logical event using the presenter:
			nextStateId = activeGUI->presenterHandleEvent(activeGUI->model, activeGUI->viewState, logicalEvent);

			// if state has changed, stop the active GUI and move to the next one:
			if (activeGUI->stateId != nextStateId) {
				if (nextStateId == QUIT) {
					break;
				}
				else {
					void* nextGuiInitData = activeGUI->stop(activeGUI);

					activeGUI = guis[nextStateId];
					activeGUI->start(activeGUI, nextGuiInitData, mainWindow);
				}
			}
			if (wait) {
				SDL_Delay(POLLING_DELAY);
			}
			
		}
		
	

	// API may be extended with a "provideInitData" flag or something similar:
	activeGUI->stop(activeGUI);



	return 0;
	//return isError;
}
