#ifndef Button_h__
#define Button_h__

#include <SDL.h>
#include <SDL_video.h>
#include "Widget.h"
#include "BitmapFont.h"
#include "../ModelInfrastructure/Model.h"

SDL_Surface* createImageFromState(WidgetState state, char* imageFile);
void changeImage(Widget* button, WidgetState state, char* imageFile);
void changeState(Widget* button, WidgetState state);
Widget* createButton(SDL_Rect rect, char* text, SDL_Surface* window, WidgetState state, char* imageFile, Uint32 bgColor, int textx, int texty, int id, Bitmapfont* bitmapfont);
void drawButton(Widget* button);
void markButton(Model* model, LogicEvent* logicalEvent, Widget* exMarkedButton, Widget* viewState, Widget* newMarkedButton);
void markButtonStart(Model* model, int buttonId, int markedId, Widget* viewState);
#endif // Button_h__
