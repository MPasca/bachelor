#include "InputProcessing.h"

InputState getNextSubstate(SDL_Keycode key)
{
	InputState substate = IDLE_INPUT;
	switch (key)
	{
	case SDLK_ESCAPE:
		substate = PAUSE;
		break;
	case SDLK_w:
		substate = ARROW_UP;
		break;
	case SDLK_a:
		substate = ARROW_LEFT;
		break;
	case SDLK_s:
		substate = ARROW_DOWN;
		break;
	case SDLK_d:
		substate = ARROW_RIGHT;
		break;
	case SDLK_SPACE:
		substate = ATTACK_INPUT;
		break;
	case SDLK_LCTRL:
		substate = INTERACT_INPUT;
		break;
	default:
		break;
	}

	return substate;
}
