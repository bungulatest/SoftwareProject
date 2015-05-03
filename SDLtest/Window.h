#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL.h>
#include <SDL_video.h>
#include "Widget.h"

SDL_Surface* createSurface();

void drawWindow(Widget* window);

Widget* createWindow(SDL_Surface* surface);

#endif /* WINDOW_H_ */
