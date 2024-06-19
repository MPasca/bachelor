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

CharacterState convert_input_to_player_state(InputState inputState)
{
	CharacterState playerState = IDLE;

	switch (inputState)
	{
	case ARROW_DOWN:
		playerState = MOVE_DOWN;
		break;
	case ARROW_UP:
		playerState = MOVE_UP;
		break;
	case ARROW_LEFT:
		playerState = MOVE_LEFT;
		break;
	case ARROW_RIGHT:
		playerState = MOVE_RIGHT;
		break;
	case INTERACT_INPUT:
		playerState = INTERACT;
		break;
	case ATTACK_INPUT:
		playerState = ATTACK;
		break;
	case IDLE_INPUT:
	default:
		playerState = IDLE;
		break;
	}

	return playerState;
}