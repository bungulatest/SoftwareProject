#ifndef BitmapFont_h__
#define BitmapFont_h__

#include <SDL.h>
#include <SDL_video.h>

// bitmap font file
#define BITMAP_FONT_FILE1 "font1.bmp"
#define BITMAP_FONT_FILE2 "font2.bmp"
#define BITMAPR 255
#define BITMAPG 255
#define BITMAPB 255

typedef struct BitmapFont {
	SDL_Surface* bitmap;
	SDL_Rect chars[256];
	int newLine; // distance between lines
	int space; // distance between words in line
} Bitmapfont;

extern Bitmapfont* bitmapfont1;
extern Bitmapfont* bitmapfont2;

Uint32 get_pixel32(int x, int y, SDL_Surface *surface);
Bitmapfont* BuildFont(SDL_Surface* fontsurface, Uint32 fontBgColor);
Bitmapfont* createFont(SDL_Surface* fontsurface, Uint32 fontBgColor);
void ShowText(int x, int y, char* text, SDL_Surface* surface, Bitmapfont* font);


void destroyFont(Bitmapfont* font);
#endif // BitmapFont_h__
