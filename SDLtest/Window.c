#include "Widget.h"
#include "Window.h"

SDL_PixelFormat* pixel_format;

SDL_Surface* createSurface() {
	SDL_Surface *w = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (w == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return NULL;
	}
	
	
	return w;
}

void drawWindow(Widget* window) {
	Uint32 color = SDL_MapRGB(pixel_format, COLOR_R, COLOR_G, COLOR_B);
	SDL_FillRect(window->window, NULL, color);
	SDL_Flip(window->window);
}

Widget* createWindow(SDL_Surface* surface) {
	SDL_Rect rect = { 0, 0, WIN_W, WIN_H };
	
	Widget* window = createWidget(WINDOW, rect, 0, NULL, REGULAR, surface, surface, NULL, drawWindow, 0, 0, -1, NULL);  // change the default id because of the getWidgetFromId() function
	return window;
}
