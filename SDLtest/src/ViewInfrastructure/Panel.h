#ifndef Panel_h__
#define Panel_h__

#include <SDL.h>
#include <SDL_video.h>


Widget* createPanel(SDL_Surface* surface, SDL_Rect rect, Uint32 bgcolor);
void drawPanel(Widget* panel);
#endif // Panel_h__