#include "Widget.h"
#include "Panel.h"


Widget* createPanel(SDL_Surface* surface, SDL_Rect rect,Uint32 bgcolor) {
	//SDL_Rect rect = { 0, 0, WIN_W, WIN_H };

	Widget* panel = createWidget(PANEL, rect, bgcolor, NULL, REGULAR, surface, surface, NULL, drawPanel, 0, 0, -1, NULL, NULL);  // change the default id because of the getWidgetFromId() function
	return panel;
}


void drawPanel(Widget* panel) {
	panel->image = SDL_CreateRGBSurface(0, panel->rect.w, panel->rect.h, pixel_format->BitsPerPixel, pixel_format->Rmask, pixel_format->Gmask, pixel_format->Bmask, pixel_format->Amask);
	SDL_FillRect(panel->image, NULL, panel->backgroundColor);
	SDL_BlitSurface(panel->image, NULL, panel->parent, &(panel->rect));
	SDL_Flip(panel->window);
}

