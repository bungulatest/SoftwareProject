#include "Label.h"
#include "BitmapFont.h"
#include "Widget.h"
#include "Button.h"
#include <stdio.h>

SDL_PixelFormat* pixel_format;

Widget* createLabel(SDL_Rect rect, char* text, SDL_Surface* window, int textx, int texty, Bitmapfont* bitmapfont, char* imageFile) {
	Uint32 color = SDL_MapRGB(pixel_format, 10, 120, 0);
	SDL_Surface* image = NULL;

	if (imageFile != NULL) {
		image = createImageFromState(REGULAR, imageFile);
	}

	return createWidget(LABEL, rect, color, text, REGULAR, window, image, NULL, drawLabel, textx, texty, -1, NULL, bitmapfont);  // change the default id because of the getWidgetFromId() function
}

void drawLabel(Widget* label) {
	if (label->image != NULL) {
		if (SDL_BlitSurface(label->image, NULL, label->parent, &(label->rect)) != 0) {
			fprintf(stderr, "ERROR: failed to set blit image: %s\n", SDL_GetError());
			exit(1);
		}
	}
	
	if (label->text != NULL) {
		ShowText(label->rect.x + label->textx, label->rect.y + label->texty, label->text, label->window, label->bitmapfont);
	}
	
}
