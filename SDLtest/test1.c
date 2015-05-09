#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>
#include "Widget.h"
#include "Window.h"
#include "Label.h"
#include "Button.h"
#include "BitmapFont.h"
#include "Panel.h"
#include "GuiFactory.h"
#include "gui.h"




SDL_PixelFormat* pixel_format;
Bitmapfont* bitmapfont1;
Bitmapfont* bitmapfont2;
Bitmapfont* bitmapfont3;
GUI* guis[STATES_COUNT];

// Should be externalized in some header file.
int isError = 0;

int main(int argc, char* args[]) {
	// Initialize SDL and make sure it quits
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	// Create window surface
	SDL_WM_SetCaption("SDL Test", "SDL Test");


	//TODO: move code to struct GUI "start" method
	SDL_Surface *mainWindow = createSurface();
	pixel_format = mainWindow->format;


	// create global bitmap fonts
	SDL_Surface* fontSurface1 = SDL_LoadBMP(BITMAP_FONT_FILE1);
	bitmapfont1 = createFont(fontSurface1, SDL_MapRGB(pixel_format, BITMAPR, BITMAPG, BITMAPB));
	SDL_Surface* fontSurface2 = SDL_LoadBMP(BITMAP_FONT_FILE2);
	bitmapfont2 = createFont(fontSurface2, SDL_MapRGB(pixel_format, BITMAPR, BITMAPG, BITMAPB));
	SDL_Surface* fontSurface3 = SDL_LoadBMP(BITMAP_FONT_FILE3);
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
	
	/*...*/

	// Starting the default/initial GUI:
	StateId nextStateId = MAIN_MENU;

	GUI* activeGUI = guis[nextStateId];
	activeGUI->start(activeGUI, NULL, mainWindow);


	//int quit = 0;
	//while (!quit) {
	//	// Poll for keyboard & mouse events
	//	SDL_Event e;
	//	while (SDL_PollEvent(&e) != 0) {
	//		switch (e.type) {
	//		case (SDL_QUIT) :
	//			quit = 1;
	//			break;
	//		}
	//	}
	//}

	while (!isError && nextStateId != QUIT) {
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0) {

			// translating the SDL event to a logical event using the view:
			LogicEvent* logicalEvent = activeGUI->viewTranslateEvent(activeGUI->viewState, &event, activeGUI->model);

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
		}
		SDL_Delay(POLLING_DELAY);
	}

	// API may be extended with a "provideInitData" flag or something similar:
	activeGUI->stop(activeGUI);

	return 0;
	//return isError;





	//Widget* window = createWindow(mainWindow);
	//SDL_Rect rectlabel = { 20, 20, 50, 50 };
	//Widget* label = createLabel(rectlabel, "bla_bla", window->image, 20, 20);
	//addChild(window, label);
	//
	//SDL_Rect rectbutton = { 400, 400, 500, 500 };
	//Widget* panel = createPanel(mainWindow, rectbutton, SDL_MapRGB(pixel_format, 60, 200, 90));
	//addChild(window, panel);

	//SDL_Rect rectbutton1 = { 0, 0, 20, 20 };
	//Widget* button = createButton(rectbutton1, "dig", window->image, REGULAR, "testbutton.bmp", SDL_MapRGB(pixel_format, 100, 200, 0), 100, 20, 1);
	//addChild(panel, button);

	//drawWidget(window);

	//int quit = 0;
	//while (!quit) {
	//	// Poll for keyboard & mouse events
	//	SDL_Event e;
	//	while (SDL_PollEvent(&e) != 0) {
	//		switch (e.type) {
	//		case (SDL_QUIT) :
	//			quit = 1;
	//			break;
	//		}
	//	}

	//	// Wait a little before redrawing
	//	SDL_Delay(10);
	//}
	//return 0;
}
