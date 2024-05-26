#include "VisLib.h"

#include<SDL.h>
#include<SDL_image.h>

#include <iostream>

// 0 - game view
// 1 - ingame view
SDL_Rect viewports[] = { {40, 30, 1000, 600}, { 400, 200, 180, 160 } };

//  code created with the help of the Lazyfoo SDL2 tutorial

// ---------------------------------- global variables

	const int SCREEN_WIDTH = 1080;
	const int SCREEN_HEIGHT = 640;

// --------------------------------- functions

// ----------- aux functions
	SDL_Texture* loadTexture(SDL_Renderer** renderer, std::string path)
	{
		SDL_Texture* newTexture = NULL;

		SDL_Surface* auxSurface = IMG_Load(path.c_str());
		if (auxSurface == NULL)
		{
			std::cerr << "Surface from image: " << path << " could not be created! (Reason: " << IMG_GetError() << "\n";
		}
		else
		{
			newTexture = SDL_CreateTextureFromSurface(*renderer, auxSurface);
			if (newTexture == NULL)
			{
				std::cerr << "Texture from image: " << path << " could not be created! (Reason: " << IMG_GetError() << ")\n";
			}
			else
			{
				SDL_FreeSurface(auxSurface);
			}

		}

		return newTexture;
	}


	// ----------- main functions

	bool fn_initialize(SDL_Window** window, SDL_Renderer** renderer, SDL_Rect** viewports, std::string title)
	{
		bool success = true;

		// initialize the window and the renderer
		*window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		else
		{
			*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
			if (*renderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}

		*viewports = (SDL_Rect*)calloc(2, sizeof(SDL_Rect));
		(*viewports)[1] = { 40, 30, 1000, 600 };	// game viewport
		(*viewports)[0] = { 400, 200, 180, 160 };	// ingame menu


		return success;
	}

	void fn_clean(SDL_Window* window, SDL_Renderer* renderer)
	{
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);

		SDL_Quit();
	}

	void fn_update(SDL_Renderer* renderer, GameElement* currentElements, SDL_Rect* viewports, int currentViewport, int numberOfGameElements)
	{
		SDL_RenderClear(renderer);	// clean the renderer from the previous frame
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		// create the viewport for the backdrop
		SDL_Rect backdropViewport = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderSetViewport(renderer, &backdropViewport);

		// background to be rendered before viewport
		std::string backdropImg = "./assets/demo_backdrop.png";
		SDL_Surface* auxSurface = IMG_Load(backdropImg.c_str());
		SDL_Texture* backdrop = SDL_CreateTextureFromSurface(renderer, auxSurface);
		SDL_FreeSurface(auxSurface);
		SDL_RenderCopy(renderer, backdrop, NULL, NULL);
		SDL_DestroyTexture(backdrop);


		// set the current viewport
		SDL_RenderSetViewport(renderer, &viewports[currentViewport]);
		if (currentViewport > 0)
		{
			SDL_RenderFillRect(renderer, &viewports[currentViewport]);
		}

		for (int i = 0; i < numberOfGameElements; i++)
		{
			SDL_Rect destinationRectangle = currentElements[i].getRectangle();
			SDL_Texture* currentTexture = loadTexture(&renderer, currentElements[i].getPath());
			SDL_RenderCopy(renderer, currentTexture, NULL, &destinationRectangle);
			SDL_DestroyTexture(currentTexture);
		}

		SDL_RenderPresent(renderer);	// show on window
	}

	// ----------- demo function
	void fn_demo()
	{
		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;
		SDL_Rect* viewports = NULL;

		if (fn_initialize(&window, &renderer, &viewports, "Demo VisLib"))
		{
			GameElement* gameElements = (GameElement*)calloc(3, sizeof(GameElement));
			gameElements[0] = GameElement(
				std::pair<int, int>{40, 30},
				std::pair<int, int>{64, 64},
				"./assets/demo.png");
			gameElements[1] = GameElement(
				std::pair<int, int>{128, 30},
				std::pair<int, int>{64, 64},
				"./assets/lizard_f_idle_anim_f0.png");
			gameElements[2] = GameElement(
				std::pair<int, int>{256, 30},
				std::pair<int, int>{64, 64},
				"./assets/lizard_m_idle_anim_f0.png");


			bool quit = false;
			SDL_Event event;
			while (!quit)
			{
				while (SDL_PollEvent(&event) != 0)
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				fn_update(renderer, gameElements, viewports, 1, 3);
			}
		}

		fn_clean(window, renderer);
	}