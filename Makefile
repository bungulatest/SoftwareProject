CFLAGS=-Wall -g -std=c99 -pedantic-errors -lm
SDL_COMPILE_FLAGS=`sdl-config --cflags`
SDL_LINK_FLAGS=`sdl-config --libs`

MAIN=src/main
GUI=src/gui

O_FILES=ListUtils.o MiniMax.o ListUtilsDemo.o MiniMaxDemo.o

all: ListUtilsDemo MiniMaxDemo CatAndMouse test1

clean:
	-rm CatAndMouse ListUtilsDemo MiniMaxDemo $(O_FILES)

ListUtilsDemo: ListUtilsDemo.o ListUtils.o
	gcc -o $@ $^ $(CFLAGS)

MiniMaxDemo: MiniMaxDemo.o MiniMax.o ListUtils.o
	gcc -o $@ $^ $(CFLAGS)
 
ListUtilsDemo.o: $(MAIN)/ListUtilsDemo.c $(MAIN)/ListUtils.h
	gcc -c $(CFLAGS) $(MAIN)/$*.c

ListUtils.o: $(MAIN)/ListUtils.c $(MAIN)/ListUtils.h
	gcc -c $(CFLAGS) $(MAIN)/$*.c

MiniMaxDemo.o: $(MAIN)/MiniMaxDemo.c $(MAIN)/MiniMax.h $(MAIN)/ListUtils.h
	gcc -c $(CFLAGS) $(MAIN)/$*.c

MiniMax.o: $(MAIN)/MiniMax.c $(MAIN)/MiniMax.h $(MAIN)/ListUtils.h
	gcc -c $(CFLAGS) $(MAIN)/$*.c

test1: test1.o ListUtils.o 
	gcc -o $@ $^ $(CFLAGS) $(SDL_COMPILE_FLAGS)

test1.o: $(MAIN)/main.c Bitmapfont.o Button.o Label.o Panel.o Widget.o Window.o
	gcc -c $(CFLAGS) $(MAIN)/$*.c $(SDL_LINK_FLAGS)
 
BitmapFont.o: $(MAIN)/Bitmapfont.c $(MAIN)/Bitmapfont.h
	gcc -c $(CFLAGS) $(MAIN)/$*.c $(SDL_COMPILE_FLAGS)
 
Button.o: $(MAIN)/Button.c $(MAIN)/Button.h
	gcc -c $(CFLAGS) $(MAIN)/$*.c $(SDL_COMPILE_FLAGS)
 
Label.o: $(MAIN)/Label.c $(MAIN)/Label.h
	gcc -c $(CFLAGS) $(MAIN)/$*.c $(SDL_COMPILE_FLAGS)
 
Panel.o: $(MAIN)/Panel.c $(MAIN)/Panel.h
	gcc -c $(CFLAGS) $(MAIN)/$*.c $(SDL_COMPILE_FLAGS)
 
Widget.o: $(MAIN)/Widget.c $(MAIN)/Widget.h
	gcc -c $(CFLAGS) $(MAIN)/$*.c $(SDL_COMPILE_FLAGS)
 
Window.o: $(MAIN)/Window.c $(MAIN)/Window.h
	gcc -c $(CFLAGS) $(MAIN)/$*.c $(SDL_COMPILE_FLAGS)