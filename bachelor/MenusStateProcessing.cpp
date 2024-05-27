#include "MenusStateProcessing.h"

// --------------------------------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------- MENU SCREENS INPUT


MainState process_menu_state(InputState inputState, Menu* crtMenu)
{
	MainState nextMainState = NO_CHANGE;
	switch (inputState)
	{
	case ARROW_DOWN:
	case ARROW_UP:
	case ARROW_LEFT:
	case ARROW_RIGHT:
		crtMenu->changeActiveButton();
		break;
	case ATTACK_INPUT:
		nextMainState = crtMenu->getActiveButton().triggerEvent();
		break;
	default:
		break;
	}

	return nextMainState;
}
