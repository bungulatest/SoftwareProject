#ifndef LABEL_H_
#define LABEL_H_

#include <SDL.h>
#include <SDL_video.h>
#include "Widget.h"

Widget* createLabel(SDL_Rect rect, char* text, SDL_Surface* window, int textx, int texty);

void drawLabel(Widget* label);

#endif /* LABEL_H_ */
