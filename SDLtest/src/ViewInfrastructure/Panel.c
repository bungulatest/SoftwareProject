#include "Widget.h"
#include "Panel.h"
#include <stdio.h>

Widget* createPanel(SDL_Surface* surface, SDL_Rect rect,Uint32 bgcolor) {

	Widget* panel = createWidget(PANEL, rect, bgcolor, NULL, REGULAR, surface, surface, NULL, drawPanel, 0, 0, -1, NULL, NULL);  // change the default id because of the getWidgetFromId() function
	return panel;
}


void drawPanel(Widget* panel) {
	if (panel->image != NULL) { // free the panel background color
		SDL_FreeSurface(panel->image);
		panel->image = NULL;
	}
	panel->image = SDL_CreateRGBSurface(0, panel->rect.w, panel->rect.h, pixel_format->BitsPerPixel, pixel_format->Rmask, pixel_format->Gmask, pixel_format->Bmask, pixel_format->Amask);
	if (panel->image == NULL) {
		fprintf(stderr, "ERROR: failed to create background image for panel: %s\n", SDL_GetError());
		exit(1);
	}

	if (SDL_FillRect(panel->image, NULL, panel->backgroundColor) != 0) {
		fprintf(stderr, "ERROR: failed to fill panel with background color: %s\n", SDL_GetError());
		exit(1);
	}
	
	if (SDL_BlitSurface(panel->image, NULL, panel->parent, &(panel->rect)) != 0) {
		fprintf(stderr, "ERROR: failed to blit panel: %s\n", SDL_GetError());
		exit(1);
	}
}

