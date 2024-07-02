#include<SDL.h>

namespace inputdcd
{
	typedef enum {
		IDLE_INPUT, ARROW_DOWN, ARROW_UP, ARROW_LEFT, ARROW_RIGHT, INTERACT_INPUT, ATTACK_INPUT, PAUSE
	} InputState;

	InputState convert_from_keyboard(SDL_Keycode key)
	{
		switch (key)
		{
		case SDLK_ESCAPE:
			return PAUSE;
		case SDLK_w:
			return ARROW_UP;
		case SDLK_a:
			return ARROW_LEFT;
		case SDLK_s:
			return ARROW_DOWN;
		case SDLK_d:
			return ARROW_RIGHT;
		case SDLK_SPACE:
			return ATTACK_INPUT;
		case SDLK_LCTRL:
			return INTERACT_INPUT;
		default:
			return IDLE_INPUT;
		}
	}
}