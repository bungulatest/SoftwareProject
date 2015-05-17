CFLAGS=-Wall -g -std=c99 -pedantic-errors -lm
SDL_COMPILE_FLAGS=`sdl-config --cflags`
SDL_LINK_FLAGS=`sdl-config --libs`

MAIN_DIR=src/main/
SCREENS_DIR=src/Screens/
SERVICES_DIR=src/Services/
VIEW_INFRASTRUCTURE_DIR=src/ViewInfrastructure/
MODEL_INFRASTRUCTURE_DIR=src/ModelInfrastructure/

WIDGET_HEADERS=$(VIEW_INFRASTRUCTURE_DIR)Button.h $(VIEW_INFRASTRUCTURE_DIR)Label.h $(VIEW_INFRASTRUCTURE_DIR)Panel.h $(VIEW_INFRASTRUCTURE_DIR)Widget.h $(VIEW_INFRASTRUCTURE_DIR)Window.h
GUI_MODEL_HEADERS = $(MODEL_INFRASTRUCTURE_DIR)gui.h $(MODEL_INFRASTRUCTURE_DIR)Model.h

LIST_UTILS_DEMO_O_FILES=ListUtils.o ListUtilsDemo.o
MINIMAX_DEMO_O_FILES=MiniMaxDemo.o MiniMax.o ListUtils.o
CAT_AND_MOUSE_O_FILES=CatAndMouse.o ConsoleMode.o ListUtils.o MiniMax.o GuiFactory.o Model.o ChooseMenu.o ChooseSkill.o ChooseType.o GameWindow.o InvalidWorld.o MainMenu.o PlayGame.o SelectWorld.o WorldBuilder.o EvaluationService.o MoveService.o WorldFileService.o BitmapFont.o Button.o Label.o Panel.o Widget.o Window.o
ALL_O_FILES=$(CAT_AND_MOUSE_O_FILES) ListUtilsDemo.o MiniMaxDemo.o

all: ListUtilsDemo MiniMaxDemo CatAndMouse

clean:
	-rm CatAndMouse ListUtilsDemo MiniMaxDemo $(ALL_O_FILES)

ListUtilsDemo: $(LIST_UTILS_DEMO_O_FILES)
	gcc -o $@ $^ $(CFLAGS)

MiniMaxDemo: $(MINIMAX_DEMO_O_FILES)
	gcc -o $@ $^ $(CFLAGS)

CatAndMouse: $(CAT_AND_MOUSE_O_FILES)
	gcc -o $@ $^ $(CFLAGS) $(SDL_LINK_FLAGS)




ListUtilsDemo.o: $(MAIN_DIR)ListUtilsDemo.c $(MAIN_DIR)ListUtils.h
	gcc -c $(CFLAGS) $(MAIN_DIR)$*.c

ListUtils.o: $(MAIN_DIR)ListUtils.c $(MAIN_DIR)ListUtils.h
	gcc -c $(CFLAGS) $(MAIN_DIR)$*.c

MiniMaxDemo.o: $(MAIN_DIR)MiniMaxDemo.c $(MAIN_DIR)MiniMax.h $(MAIN_DIR)ListUtils.h
	gcc -c $(CFLAGS) $(MAIN_DIR)$*.c

MiniMax.o: $(MAIN_DIR)MiniMax.c $(MAIN_DIR)MiniMax.h $(MAIN_DIR)ListUtils.h $(MAIN_DIR)MiniMaxWithAlphaBeta.h
	gcc -c $(CFLAGS) $(MAIN_DIR)$*.c



CatAndMouse.o: $(MAIN_DIR)CatAndMouse.c $(VIEW_INFRASTRUCTURE_DIR)Widget.h $(MODEL_INFRASTRUCTURE_DIR)GuiFactory.h $(MODEL_INFRASTRUCTURE_DIR)gui.h $(MAIN_DIR)ConsoleMode.h $(MAIN_DIR)ErrorHandling.h
	gcc -c $(CFLAGS) $(MAIN_DIR)$*.c $(SDL_COMPILE_FLAGS)

ConsoleMode.o: $(MAIN_DIR)ConsoleMode.c $(MAIN_DIR)ConsoleMode.h $(MODEL_INFRASTRUCTURE_DIR)Model.h $(SERVICES_DIR)EvaluationService.h $(SERVICES_DIR)WorldFileService.h
	gcc -c $(CFLAGS) $(MAIN_DIR)$*.c $(SDL_COMPILE_FLAGS)

ErrorHandling.o: $(MAIN_DIR)ErrorHandling.c $(MAIN_DIR)ErrorHandling.h $(GUI_MODEL_HEADERS) $(VIEW_INFRASTRUCTURE_DIR)Widget.h

GuiFactory.o: $(MODEL_INFRASTRUCTURE_DIR)GuiFactory.c $(MODEL_INFRASTRUCTURE_DIR)GuiFactory.h $(SCREENS_DIR)MainMenu.h $(SCREENS_DIR)ChooseType.h $(SCREENS_DIR)ChooseSkill.h $(SCREENS_DIR)SelectWorld.h $(SCREENS_DIR)PlayGame.h $(SCREENS_DIR)WorldBuilder.h $(SCREENS_DIR)InvalidWorld.h
	gcc -c $(CFLAGS) $(MODEL_INFRASTRUCTURE_DIR)$*.c $(SDL_COMPILE_FLAGS)

Model.o: $(MODEL_INFRASTRUCTURE_DIR)Model.c $(MODEL_INFRASTRUCTURE_DIR)Model.h
	gcc -c $(CFLAGS) $(MODEL_INFRASTRUCTURE_DIR)$*.c $(SDL_COMPILE_FLAGS)



ChooseMenu.o: $(SCREENS_DIR)ChooseMenu.c $(SCREENS_DIR)ChooseMenu.h $(MODEL_INFRASTRUCTURE_DIR)Model.h $(VIEW_INFRASTRUCTURE_DIR)Widget.h $(SCREENS_DIR)ScreenConstants.h
	gcc -c $(CFLAGS) $(SCREENS_DIR)$*.c $(SDL_COMPILE_FLAGS)

ChooseSkill.o: $(SCREENS_DIR)ChooseSkill.c $(SCREENS_DIR)ChooseSkill.h $(WIDGET_HEADERS) $(SCREENS_DIR)ChooseMenu.h $(GUI_MODEL_HEADERS) $(SERVICES_DIR)MoveService.h $(SCREENS_DIR)ScreenConstants.h
	gcc -c $(CFLAGS) $(SCREENS_DIR)$*.c $(SDL_COMPILE_FLAGS)
	
ChooseType.o: $(SCREENS_DIR)ChooseType.c $(SCREENS_DIR)ChooseType.h $(WIDGET_HEADERS) $(SCREENS_DIR)ChooseMenu.h $(GUI_MODEL_HEADERS) $(SERVICES_DIR)MoveService.h $(SCREENS_DIR)ScreenConstants.h
	gcc -c $(CFLAGS) $(SCREENS_DIR)$*.c $(SDL_COMPILE_FLAGS)

GameWindow.o: $(SCREENS_DIR)GameWindow.c $(SCREENS_DIR)GameWindow.h $(WIDGET_HEADERS) $(MODEL_INFRASTRUCTURE_DIR)Model.h $(VIEW_INFRASTRUCTURE_DIR)BitmapFont.h $(SERVICES_DIR)WorldFileService.h $(SERVICES_DIR)MoveService.h $(SCREENS_DIR)ScreenConstants.h
	gcc -c $(CFLAGS) $(SCREENS_DIR)$*.c $(SDL_COMPILE_FLAGS)

InvalidWorld.o: $(SCREENS_DIR)InvalidWorld.c $(SCREENS_DIR)InvalidWorld.h $(WIDGET_HEADERS) $(MODEL_INFRASTRUCTURE_DIR)Model.h $(SERVICES_DIR)WorldFileService.h $(SCREENS_DIR)ChooseMenu.h $(SCREENS_DIR)ScreenConstants.h
	gcc -c $(CFLAGS) $(SCREENS_DIR)$*.c $(SDL_COMPILE_FLAGS)

MainMenu.o: $(SCREENS_DIR)MainMenu.c $(SCREENS_DIR)MainMenu.h $(WIDGET_HEADERS) $(MODEL_INFRASTRUCTURE_DIR)Model.h $(SCREENS_DIR)ChooseMenu.h $(SCREENS_DIR)GameWindow.h $(SCREENS_DIR)ScreenConstants.h
	gcc -c $(CFLAGS) $(SCREENS_DIR)$*.c $(SDL_COMPILE_FLAGS)

PlayGame.o: $(SCREENS_DIR)PlayGame.c $(SCREENS_DIR)PlayGame.h $(WIDGET_HEADERS) $(SCREENS_DIR)ChooseMenu.h $(SCREENS_DIR)GameWindow.h $(MODEL_INFRASTRUCTURE_DIR)Model.h $(SERVICES_DIR)WorldFileService.h $(SERVICES_DIR)EvaluationService.h $(SERVICES_DIR)MoveService.h $(SCREENS_DIR)ScreenConstants.h
	gcc -c $(CFLAGS) $(SCREENS_DIR)$*.c $(SDL_COMPILE_FLAGS)
	
SelectWorld.o: $(SCREENS_DIR)SelectWorld.c $(SCREENS_DIR)SelectWorld.h $(WIDGET_HEADERS) $(GUI_MODEL_HEADERS) $(SERVICES_DIR)WorldFileService.h $(SCREENS_DIR)ChooseMenu.h $(SCREENS_DIR)ScreenConstants.h
	gcc -c $(CFLAGS) $(SCREENS_DIR)$*.c $(SDL_COMPILE_FLAGS)
	
WorldBuilder.o: $(SCREENS_DIR)WorldBuilder.c $(SCREENS_DIR)WorldBuilder.h $(WIDGET_HEADERS) $(SCREENS_DIR)ChooseMenu.h $(MODEL_INFRASTRUCTURE_DIR)Model.h $(SCREENS_DIR)GameWindow.h $(SERVICES_DIR)MoveService.h $(SCREENS_DIR)SelectWorld.h $(SERVICES_DIR)WorldFileService.h $(SCREENS_DIR)ScreenConstants.h
	gcc -c $(CFLAGS) $(SCREENS_DIR)$*.c $(SDL_COMPILE_FLAGS)
	
	
	
EvaluationService.o: $(SERVICES_DIR)EvaluationService.c $(SERVICES_DIR)EvaluationService.h $(MODEL_INFRASTRUCTURE_DIR)Model.h $(SERVICES_DIR)WorldFileService.h $(MAIN_DIR)ListUtils.h $(MAIN_DIR)MiniMax.h $(SERVICES_DIR)MoveService.h
	gcc -c $(CFLAGS) $(SERVICES_DIR)$*.c $(SDL_COMPILE_FLAGS)
	
MoveService.o: $(SERVICES_DIR)MoveService.c $(SERVICES_DIR)MoveService.h $(MODEL_INFRASTRUCTURE_DIR)Model.h $(SERVICES_DIR)WorldFileService.h $(SERVICES_DIR)EvaluationService.h $(MAIN_DIR)ListUtils.h $(MAIN_DIR)MiniMax.h
	gcc -c $(CFLAGS) $(SERVICES_DIR)$*.c $(SDL_COMPILE_FLAGS)
	
WorldFileService.o: $(SERVICES_DIR)WorldFileService.c $(SERVICES_DIR)WorldFileService.h $(MODEL_INFRASTRUCTURE_DIR)Model.h $(SERVICES_DIR)MoveService.h
	gcc -c $(CFLAGS) $(SERVICES_DIR)$*.c $(SDL_COMPILE_FLAGS)
	

	
BitmapFont.o: $(VIEW_INFRASTRUCTURE_DIR)BitmapFont.c $(VIEW_INFRASTRUCTURE_DIR)BitmapFont.h
	gcc -c $(CFLAGS) $(VIEW_INFRASTRUCTURE_DIR)$*.c $(SDL_COMPILE_FLAGS)
 
Button.o: $(VIEW_INFRASTRUCTURE_DIR)Button.c $(VIEW_INFRASTRUCTURE_DIR)Button.h $(VIEW_INFRASTRUCTURE_DIR)BitmapFont.h $(VIEW_INFRASTRUCTURE_DIR)Widget.h $(MODEL_INFRASTRUCTURE_DIR)Model.h
	gcc -c $(CFLAGS) $(VIEW_INFRASTRUCTURE_DIR)$*.c $(SDL_COMPILE_FLAGS)
 
Label.o: $(VIEW_INFRASTRUCTURE_DIR)Label.c $(VIEW_INFRASTRUCTURE_DIR)Label.h $(VIEW_INFRASTRUCTURE_DIR)BitmapFont.h $(VIEW_INFRASTRUCTURE_DIR)Widget.h $(VIEW_INFRASTRUCTURE_DIR)Button.h
	gcc -c $(CFLAGS) $(VIEW_INFRASTRUCTURE_DIR)$*.c $(SDL_COMPILE_FLAGS)
 
Panel.o: $(VIEW_INFRASTRUCTURE_DIR)Panel.c $(VIEW_INFRASTRUCTURE_DIR)Panel.h $(VIEW_INFRASTRUCTURE_DIR)Widget.h
	gcc -c $(CFLAGS) $(VIEW_INFRASTRUCTURE_DIR)$*.c $(SDL_COMPILE_FLAGS)
 
Widget.o: $(VIEW_INFRASTRUCTURE_DIR)Widget.c $(VIEW_INFRASTRUCTURE_DIR)Widget.h $(MAIN_DIR)ListUtils.h $(VIEW_INFRASTRUCTURE_DIR)BitmapFont.h $(VIEW_INFRASTRUCTURE_DIR)Window.h
	gcc -c $(CFLAGS) $(VIEW_INFRASTRUCTURE_DIR)$*.c $(SDL_COMPILE_FLAGS)
 
Window.o: $(VIEW_INFRASTRUCTURE_DIR)Window.c $(VIEW_INFRASTRUCTURE_DIR)Window.h $(VIEW_INFRASTRUCTURE_DIR)Widget.h
	gcc -c $(CFLAGS) $(VIEW_INFRASTRUCTURE_DIR)$*.c $(SDL_COMPILE_FLAGS)
