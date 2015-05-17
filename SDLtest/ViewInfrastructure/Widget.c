#include "Widget.h"
#include "../main/ListUtils.h"
#include "BitmapFont.h"
#include "Window.h"


//getters
WidgetType getType(Widget* widget) {
	return widget->type;
}

SDL_Rect getRect(Widget* widget) {
	return widget->rect;
}

SDL_Surface* getWindow(Widget* widget) {
	return widget->window;
}

SDL_Surface* getImage(Widget* widget) {
	return widget->image;
}

SDL_Surface* getParent(Widget* widget) {
	return widget->parent;
}

ListRef getChildren(Widget* widget) {
	return widget->children;
}

Draw getDraw(Widget* widget) {
	return widget->draw;
}

SDL_Surface* createSurface() {
	SDL_Surface *w = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (w == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return NULL;
	}


	return w;
}


Widget* createWidget(WidgetType type, SDL_Rect rect, Uint32 color, char* text, WidgetState state, SDL_Surface* window, SDL_Surface* image, SDL_Surface* parent, Draw draw, int textx, int texty, int id, char* imageFile, Bitmapfont* bitmapfont) {
	Widget* widget = (Widget*)malloc(sizeof(Widget));
	if (widget == NULL) {
		perror("ERROR: malloc failed\n");
		exit(1);
	}
	widget->type = type;
	widget->rect = rect;
	widget->backgroundColor = color;
	widget->state = state;
	widget->window = window;
	widget->image = image;
	widget->parent = parent;
	widget->draw = draw;
	widget->textx = textx;
	widget->texty = texty;
	widget->id = id;
	widget->imageFile = imageFile;
	widget->bitmapfont = bitmapfont;
	widget->isVisible = 1;
	widget->isFree = 0;

	widget->children = NULL;

	if (text != NULL) {
		widget->text = (char*)malloc(sizeof(char) * (strlen(text) + 1));
		strcpy(widget->text, text);
	}
	else {
		widget->text = text;
	}
	

	return widget;
}

void drawWidget(Widget* widget) {
	drawWidgetRec(widget);
	SDL_Flip(widget->window);
}

void drawWidgetRec(Widget* widget) {
	if (widget->isVisible) {
		widget->draw(widget);
	}
	ListRef currNode = widget->children;
	Widget* currWidget;
	while (currNode != NULL) {
		currWidget = headData(currNode);
		drawWidgetRec(currWidget);
		currNode = tail(currNode);
	}
}

void addChild(Widget* parentWidget, Widget* childWidget) {
	if (isEmpty(parentWidget->children)) {
		parentWidget->children = newList(NULL);
	}
	append(parentWidget->children, childWidget);

	childWidget->parent = parentWidget->image;

	childWidget->rect.x += parentWidget->rect.x;
	childWidget->rect.y += parentWidget->rect.y;


	//TODO: check edge cases like child widget overflows the parent width
}

int inBoundary(int x, int y, Widget* widget) {
	if ((x > widget->rect.x) && (x < widget->rect.x + widget->rect.w) && (y > widget->rect.y) && (y < widget->rect.y + widget->rect.h))
		return 1;
	else
		return 0;
}


Widget* getWidgetFromPos(int x, int y, Widget* widget) {
	if (widget->children == NULL) {
		return widget;
	}
	ListRef currNode = widget->children;
	Widget* currWidget;
	while (currNode != NULL) {
		currWidget = headData(currNode);

		if (inBoundary(x, y, currWidget)) {
			currWidget = getWidgetFromPos(x, y, currWidget);
			return currWidget;
		}
		currNode = tail(currNode);
	}
	return widget;
}

Widget* getWidgetFromId(int id, Widget* widget) {
	Widget* correctWidget = NULL;
	ListRef currNode;
	Widget* currWidget;

	if (widget->id == id) {
		correctWidget = widget;
	}

	else {
		currNode = widget->children;
		while (currNode != NULL) {
			currWidget = headData(currNode);
			correctWidget = getWidgetFromId(id, currWidget);

			if (correctWidget != NULL) {
				break;
			}

			currNode = tail(currNode);
		}
	}
	
	return correctWidget;
}

void setText(Widget* widget, char* text) {
	widget->text = (char*)realloc(widget->text, sizeof(char)*(strlen(text) + 1));
	strcpy(widget->text, text);
}

void freeTree(Widget* widget) {
	if (widget == NULL) {
		return;
	}

	ListRef currNode = widget->children;
	Widget* currWidget;
	while (currNode != NULL) {
		currWidget = headData(currNode);
		freeTree(currWidget);
		freeWidget(currWidget);
		currNode = tail(currNode);
	}

	widget->isFree = 1;
}

void freeWidget(Widget* widget) {
	if (widget->image != NULL) {
		SDL_FreeSurface(widget->image);
	}
	if (widget->parent == NULL) {
		//destroyFont(widget->bitmapfont);
		SDL_FreeSurface(widget->window);
	}
	if (widget->text != NULL) {
		free(widget->text);
	}
	free(widget);
}
