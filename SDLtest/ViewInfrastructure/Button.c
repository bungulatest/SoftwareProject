#include "Button.h"
#include "BitmapFont.h"
#include "Widget.h"
#include "../ModelInfrastructure/Model.h"

#define REGULAR_DIR "images/regular"
#define MARKED_DIR "images/marked"
#define DISABLED_DIR "images/disabled"

SDL_PixelFormat* pixel_format;

SDL_Surface* createImageFromState(WidgetState state, char* imageFile) {
	SDL_Surface* image = NULL;
	char imageFilePath[1000];

	if (state == REGULAR) {
		strcpy(imageFilePath, REGULAR_DIR);

	}
	else if (state == MARKED) {
		strcpy(imageFilePath, MARKED_DIR);
	}
	else if (state == DISABLED) {
		strcpy(imageFilePath, DISABLED_DIR);
	}

	strcat(imageFilePath, "/");
	strcat(imageFilePath, imageFile);
	image = SDL_LoadBMP(imageFilePath);

	SDL_Surface* formattedImage = SDL_DisplayFormat(image);
	SDL_FreeSurface(image);

	return formattedImage;
}

void changeImage(Widget* button, WidgetState state, char* imageFile) {
	SDL_FreeSurface(button->image);
	button->image = createImageFromState(state, imageFile);
	button->imageFile = imageFile;
}

void changeState(Widget* button, WidgetState state) {
	button->image = createImageFromState(state, button->imageFile);
}

Widget* createButton(SDL_Rect rect, char* text, SDL_Surface* window, WidgetState state, char* imageFile, Uint32 bgColor, int textx, int texty, int id, Bitmapfont* bitmapfont) {

	SDL_Surface* image = createImageFromState(state, imageFile);

	SDL_SetColorKey(image, SDL_SRCCOLORKEY, bgColor);


	return createWidget(BUTTON, rect, bgColor, text, state, window, image, NULL, drawButton, textx, texty, id, imageFile, bitmapfont);
	//TODO: fix warning
}

void drawButton(Widget* button) {
	SDL_BlitSurface(button->image, NULL, button->parent, &(button->rect));
	if (button->text != NULL)
	{
		ShowText(button->rect.x + button->textx, button->rect.y + button->texty, button->text, button->window, button->bitmapfont);
	}
	//SDL_Flip(button->window);
}

void markButton(Model* model, LogicEvent* logicalEvent, Widget* exMarkedButton, Widget* viewState, Widget* newMarkedButton) {
	model->markedButton = logicalEvent->buttonId;
	exMarkedButton = getWidgetFromId(logicalEvent->markedId, viewState);
	exMarkedButton->state = REGULAR;
	changeImage(exMarkedButton, REGULAR, exMarkedButton->imageFile);
	newMarkedButton = getWidgetFromId(logicalEvent->buttonId, viewState);
	newMarkedButton->state = MARKED;
	changeImage(newMarkedButton, MARKED, newMarkedButton->imageFile);
}


void markButtonStart(Model* model, int buttonId, int markedId, Widget* viewState) {
	Widget* exMarkedButton = NULL;
	Widget* newMarkedButton = NULL;

	model->markedButton = buttonId;
	exMarkedButton = getWidgetFromId(markedId, viewState);
	exMarkedButton->state = REGULAR;
	changeImage(exMarkedButton, REGULAR, exMarkedButton->imageFile);
	newMarkedButton = getWidgetFromId(buttonId, viewState);
	newMarkedButton->state = MARKED;
	changeImage(newMarkedButton, MARKED, newMarkedButton->imageFile);
}