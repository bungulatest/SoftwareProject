#include "Widget.h"
#include "Window.h"
#include <stdio.h>

SDL_PixelFormat* pixel_format;



void drawWindow(Widget* window) {
	Uint32 color = SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B);
	if (SDL_FillRect(window->window, NULL, color) != 0) {
		fprintf(stderr, "ERROR: failed to fill window with background color: %s\n", SDL_GetError());
		exit(1);
	}
}

Widget* createWindow(SDL_Surface* surface) {
	SDL_Rect rect = { 0, 0, WIN_W, WIN_H };
	
	Widget* window = createWidget(WINDOW, rect, 0, NULL, REGULAR, surface, surface, NULL, drawWindow, 0, 0, -1, NULL, NULL);  // change the default id because of the getWidgetFromId() function
	return window;
}
