#include "Label.h"
#include "BitmapFont.h"
#include "Widget.h"

SDL_PixelFormat* pixel_format;
Bitmapfont* bitmapfont;

Widget* createLabel(SDL_Rect rect, char* text, SDL_Surface* window, int textx, int texty) {
	Uint32 color = SDL_MapRGB(pixel_format, 10, 120, 0);
	return createWidget(LABEL, rect, color, text, REGULAR, window, NULL, NULL, drawLabel, textx, texty, -1, NULL);  // change the default id because of the getWidgetFromId() function
	//TODO: fix warning
}

void drawLabel(Widget* label) {
	//label->image = SDL_CreateRGBSurface(0, label->rect.w, label->rect.h, pixel_format->BitsPerPixel, pixel_format->Rmask, pixel_format->Gmask, pixel_format->Bmask, pixel_format->Amask);
	//SDL_FillRect(label->image, NULL, label->backgroundColor);
	//SDL_BlitSurface(label->image, NULL, label->parent, &(label->rect));
	ShowText(label->rect.x + label->textx, label->rect.y + label->texty, label->text, label->window, bitmapfont);
	SDL_Flip(label->window);
}
