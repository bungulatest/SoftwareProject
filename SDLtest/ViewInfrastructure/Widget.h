#ifndef WIDGET_H_
#define WIDGET_H_

#include <SDL.h>
#include <SDL_video.h>
#include "../main/ListUtils.h"
#include "BitmapFont.h"

//window dimensions and background color
#define WIN_W 800
#define WIN_H 800
#define COLOR_R 255	
#define COLOR_G 255
#define COLOR_B 255

extern SDL_PixelFormat* pixel_format;

typedef enum {
	WINDOW,
	PANEL,
	LABEL,
	BUTTON
} WidgetType;

typedef enum {
	REGULAR,
	MARKED,
	DISABLED
} WidgetState;


typedef struct widget {
	WidgetType type;
	int id;
	SDL_Rect rect; //relative position to the parent. 
	//int absx; //absolute position of x.
	//int absy;
	Uint32 backgroundColor;
	char* text;
	WidgetState state;
	SDL_Surface* window; // the root surface
	SDL_Surface* image;
	char* imageFile;
	SDL_Surface* parent;
	ListRef children;
	void(*draw)(struct widget*); // function pointer to draw function
	int textx; // position of text label
	int texty;
	Bitmapfont* bitmapfont;
	int isVisible;
} Widget;

typedef void(*Draw)(Widget*);

//getters
WidgetType getType(Widget* widget);

SDL_Rect getRect(Widget* widget);

SDL_Surface* getWindow(Widget* widget);

SDL_Surface* getImage(Widget* widget);

SDL_Surface* getParent(Widget* widget);

ListRef getChildren(Widget* widget);

Draw getDraw(Widget* widget);



Widget* createWidget(WidgetType type, SDL_Rect rect, Uint32 color, char* text, WidgetState state, SDL_Surface* window, SDL_Surface* image, SDL_Surface* parent, Draw draw, int textx, int texty, int id, char* imageFile, Bitmapfont* bitmapfont);
void drawWidget(Widget* widget);

void drawWidgetRec(Widget* widget);

void addChild(Widget* parentWidget, Widget* childWidget);

int inBoundary(int x, int y, Widget* widget);

Widget* getWidgetFromPos(int x, int y, Widget* widget);

Widget* getWidgetFromId(int id, Widget* widget);
void setText(Widget* widget, char* text);
void freeTree(Widget* widget);

void freeWidget(Widget* widget);

#endif /* WIDGET_H_ */
