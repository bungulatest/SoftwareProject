#include "BitmapFont.h"
#include <string.h>
#include <stdio.h>


Uint32 GetPixel(int x, int y, SDL_Surface *surface) {
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;

	//Get the pixel requested
	return pixels[(y * surface->w) + x];
}

void initFont(Bitmapfont* font, SDL_Surface* fontsurface, Uint32 fontBgColor) {
	int cellW, cellH, top, baseA;
	int rows, cols;
	int pCol, pRow, pCol_w, pRow_w, pX, pY;
	int currentChar;
	int t;

	

	font->bitmap = fontsurface;
	
	//Set the cell dimensions
	cellW = font->bitmap->w / 16;
	cellH = font->bitmap->h / 16;

	//New line variables
	top = cellH;
	baseA = cellH;

	//The current character we're setting
	currentChar = 0;

	//Go through the cell rows
	for (rows = 0; rows < 16; rows++) {
		//Go through the cell columns
		for (cols = 0; cols < 16; cols++) {
			//Set the character offset
			font->chars[currentChar].x = cellW * cols;
			font->chars[currentChar].y = cellH * rows;

			//Set the dimensions of the character
			font->chars[currentChar].w = cellW;
			font->chars[currentChar].h = cellH;

			//Find Left Side
			//Go through pixel columns
			for (pCol = 0; pCol < cellW; pCol++) {
				//Go through pixel rows
				for (pRow = 0; pRow < cellH; pRow++) {
					//Get the pixel offsets
					pX = (cellW * cols) + pCol;
					pY = (cellH * rows) + pRow;

					//If a non colorkey pixel is found
					if (GetPixel(pX, pY, font->bitmap) != fontBgColor) {
						//Set the x offset
						font->chars[currentChar].x = pX;

						//Break the loops
						pCol = cellW;
						pRow = cellH;
					}
				}
			}

			//Find Right Side
			//Go through pixel columns
			for (pCol_w = cellW - 1; pCol_w >= 0; pCol_w--)
				//Go through pixel rows
			{
				for (pRow_w = 0; pRow_w < cellH; pRow_w++) {
					//Get the pixel offsets
					pX = (cellW * cols) + pCol_w;
					pY = (cellH * rows) + pRow_w;

					//If a non colorkey pixel is found
					if (GetPixel(pX, pY, font->bitmap) != fontBgColor) {
						//Set the width
						font->chars[currentChar].w = (pX - font->chars[currentChar].x) + 1;

						//Break the loops
						pCol_w = -1;
						pRow_w = cellH;
					}
				}
			}

			//Find Top
			//Go through pixel rows
			for (pRow = 0; pRow < cellH; pRow++) {
				//Go through pixel columns
				for (pCol = 0; pCol < cellW; pCol++) {
					//Get the pixel offsets
					int pX = (cellW * cols) + pCol;
					int pY = (cellH * rows) + pRow;

					//If a non colorkey pixel is found
					if (GetPixel(pX, pY, font->bitmap) != fontBgColor) {
						//If new top is found
						if (pRow < top) {
							top = pRow;
						}

						//Break the loops
						pCol = cellW;
						pRow = cellH;
					}
				}
			}

			//Find Bottom of A
			if (currentChar == 'A') {
				//Go through pixel rows
				for (pRow = cellH - 1; pRow >= 0; pRow--) {
					//Go through pixel columns
					for (pCol = 0; pCol < cellW; pCol++) {
						//Get the pixel offsets
						pX = (cellW * cols) + pCol;
						pY = (cellH * rows) + pRow;

						//If a non colorkey pixel is found
						if (GetPixel(pX, pY, font->bitmap) != fontBgColor) {
							//Bottom of a is found
							baseA = pRow;

							//Break the loops
							pCol = cellW;
							pRow = -1;
						}
					}
				}
			}

			//Go to the next character
			currentChar++;
		}
	}

	//Calculate space
	font->space = cellW / 2;

	//Calculate new line
	font->newLine = baseA - top;

	//Lop off excess top pixels
	for (t = 0; t < 256; t++) {
		font->chars[t].y += top;
		font->chars[t].h -= top;
	}

}

Bitmapfont* createFont(SDL_Surface* fontsurface, Uint32 fontBgColor) {
	Bitmapfont* font = (Bitmapfont*)(malloc(sizeof(Bitmapfont)));
	if (font == NULL) {
		perror("ERROR: malloc failed\n");
		exit(1);
	}

	SDL_Surface* formattedFontSurface = SDL_DisplayFormat(fontsurface);
	if (formattedFontSurface == NULL) {
		fprintf(stderr, "ERROR: failed to format bitmap font: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_FreeSurface(fontsurface);

	if (SDL_SetColorKey(formattedFontSurface, SDL_SRCCOLORKEY, fontBgColor) != 0) {
		fprintf(stderr, "ERROR: failed to set color key in bitmap font: %s\n", SDL_GetError());
		exit(1);
	}

	initFont(font, formattedFontSurface, fontBgColor);
	return font;
}

void ShowText(int x, int y, char* text, SDL_Surface* surface, Bitmapfont* font) {
	int tempx = x;
	int tempy = y;
	int i;
	int ascii;
	SDL_Rect offset;

	if (font != NULL) {
		for (i = 0; i < (int) strlen(text); i++) {
			if (text[i] == ' ') {
				tempx += font->space;
			}

			else if (text[i] == '\n') {
				tempy += font->newLine;
				tempx = x;
			}

			else {
				ascii = (char)text[i];
				offset = font->chars[ascii];
				offset.x = tempx;
				offset.y = tempy;
				if (SDL_BlitSurface(font->bitmap, &(font->chars[ascii]), surface, &offset) != 0) {
					fprintf(stderr, "ERROR: failed to set blit bitmap font: %s\n", SDL_GetError());
					exit(1);
				}
				tempx += font->chars[ascii].w + 1;
			}
		}
	}
}

void destroyFont(Bitmapfont* font) {
	SDL_FreeSurface(font->bitmap);
	free(font);
}
