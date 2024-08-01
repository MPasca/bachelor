#pragma once
#include<SDL.h>

namespace inputdcd
{
	typedef enum {
		IDLE_INPUT, ARROW_DOWN, ARROW_UP, ARROW_LEFT, ARROW_RIGHT, INTERACT_INPUT, ATTACK_INPUT, PAUSE
	} InputState;

	InputState convert_from_keyboard(SDL_Keycode key);
}

	