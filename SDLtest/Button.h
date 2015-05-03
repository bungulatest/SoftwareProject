#ifndef Button_h__
#define Button_h__

#include <SDL.h>
#include <SDL_video.h>
#include "Widget.h"

SDL_Surface* createImageFromState(WidgetState state, char* imageFile);
void changeImage(Widget* button, WidgetState state, char* imageFile);
Widget* createButton(SDL_Rect rect, char* text, SDL_Surface* window, WidgetState state, char* imageFile, Uint32 bgColor, int textx, int texty, int id);
void drawButton(Widget* button);
#endif // Button_h__
