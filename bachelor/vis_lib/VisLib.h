#include "GameElement.h"

	typedef enum ViewportType {
		GAME_VIEW, TITLE_VIEW, INGAME_VIEW
	}ViewportType;

	bool fn_initialize(SDL_Window** window, SDL_Renderer** renderer, std::string title);

	void fn_clean(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect* viewport);

	void fn_update(SDL_Renderer* renderer, GameElement* currentElements, SDL_Rect* viewports, int currentViewport, int numberOfGameElements);

	void fn_demo();
