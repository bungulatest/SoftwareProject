#ifndef ChooseSkill_h__
#define ChooseSkill_h__

#include <SDL.h>
#include <SDL_video.h>
#include "gui.h"

#define CHOOSE_CAT_SKILL_TITLE "Choose Cat Skill Level:"
#define CHOOSE_MOUSE_SKILL_TITLE "Choose Mouse Skill Level:"
#define SKILL_DONE_TEXT "Done"
#define SKILL_BACK_TEXT "Back"
#define CHOOSE_SKILL_NUM_BUTTONS 3
#define DEFAULT_SKILL 5
#define MAX_SKILL 9
#define MIN_SKILL 1

typedef enum {
	BUTTON_SKILL_LEVEL,
	BUTTON_SKILL_DONE,
	BUTTON_SKILL_BACK,
	BUTTON_SKILL_INCREASE,
	BUTTON_SKILL_DECREASE
} ChooseSkillButtons;


void createGUIChooseSkill(GUI* gui);
Widget* initializeChooseSkillWindow(SDL_Surface* windowSurface);
void changeSkillButton(Widget* viewState);
void chooseSkillStart(GUI* gui, Model* initData, SDL_Surface* windowSurface);
LogicEvent* chooseSkillTranslateUpOrDownEvent(Widget* viewState, SDL_Event* event, Model* model);
LogicEvent* chooseSkillTranslateEvent(Widget* viewState, SDL_Event* event, Model* model);


StateId chooseSkillHandleEvent(Model* model, Widget* viewState, LogicEvent* logicalEvent);
void* chooseSkillStop(GUI* gui, StateId nextStateId);
#endif // ChooseSkill_h__
