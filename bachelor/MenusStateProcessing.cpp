#include "MenusStateProcessing.h"

MainState process_menu_state(InputState inputState, Menu* crtMenu)
{
	MainState nextMainState = SAME_STATE;
	switch (inputState)
	{
	case ARROW_DOWN:
	case ARROW_RIGHT:
		crtMenu->changeActiveButton(1);
		break;
	case ARROW_UP:
	case ARROW_LEFT:
		crtMenu->changeActiveButton(-1);
		break;
	case ATTACK_INPUT:
		nextMainState = crtMenu->getActiveButton().triggerEvent();
		std::cout << "nextMainState: " << nextMainState << "\n";
		break;
	default:
		break;
	}

	return nextMainState;
}
