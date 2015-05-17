#include "Label.h"
#include "BitmapFont.h"
#include "Widget.h"
#include "Button.h"

SDL_PixelFormat* pixel_format;

Widget* createLabel(SDL_Rect rect, char* text, SDL_Surface* window, int textx, int texty, Bitmapfont* bitmapfont, char* imageFile) {
	Uint32 color = SDL_MapRGB(pixel_format, 10, 120, 0);
	SDL_Surface* image = NULL;

	if (imageFile != NULL) {
		image = createImageFromState(REGULAR, imageFile);
	}

	return createWidget(LABEL, rect, color, text, REGULAR, window, image, NULL, drawLabel, textx, texty, -1, NULL, bitmapfont);  // change the default id because of the getWidgetFromId() function
	//TODO: fix warning
}

void drawLabel(Widget* label) {
	if (label->image != NULL) {
		SDL_BlitSurface(label->image, NULL, label->parent, &(label->rect));
	}
	
	if (label->text != NULL) {
		ShowText(label->rect.x + label->textx, label->rect.y + label->texty, label->text, label->window, label->bitmapfont);
	}
	
	//SDL_Flip(label->window);
}
