#include "GuiFactory.h"
#include "MainMenu.h"
#include "ChooseType.h"
#include "ChooseSkill.h"



GUI createGUIForState(StateId stateId) {
	GUI gui;
	gui.stateId = stateId;

	switch (stateId) {
	case MAIN_MENU:
		createGUIMain(&gui);
		break;
	case CHOOSE_CAT:
		createGUIChooseType(&gui);
		break;
	case CHOOSE_MOUSE:
		createGUIChooseType(&gui);
		break;
	case LOAD_GAME:
		//createGuiLoad(&gui);
		break;
	case CAT_CHOOSE_SKILL:
		createGUIChooseSkill(&gui);
		break;
	case MOUSE_CHOOSE_SKILL:
		createGUIChooseSkill(&gui);
		break;
	default:
		break;
	}
	return gui;
}

