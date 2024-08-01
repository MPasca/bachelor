#include "GameElement.h"

namespace vislib
{
	bool fn_initialize(SDL_Window** window, SDL_Renderer** renderer, std::string title);

	void fn_clean(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect* viewport);

	void fn_update(SDL_Renderer* renderer, GameElement* currentElements, SDL_Rect* viewports, int numberOfGameElements);

	void fn_demo();
}
