#include<iostream>

#include "maze_gen/MaGe.h"
#include "input_processing/InputProcessing.h"
#include "StateProcessing.h"
#include "vis_lib/VisLib.h"
#include "vis_lib/GameElement.h"

#include "Utils.h"
#include "GameChunk.h"
#include "Character.h"

#include<SDL.h>
#undef main

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect* viewports = NULL;
int timer = 0;

GameChunk* gameChunks;
std::pair<int, int>* portals;
int numberOfPortals = 0;

MainState currentProgramState;
MainState previousProgramState;

GameElement* gameElements;

// ----- button title screen
Button* titleButtons = NULL;
// Backdrop viewBackdrop

Button* endgameButtons = NULL;
// Backdrop winBackdrop
// Backdrop loseBackdrop

// ------ in-game elements
PlayerCharacter mainCharacter;
NonplayerCharacter npCharacter;


// ---------------------------------------------------- INIT GAME SURFACE

void dfs_visit(mage::Node* crtNode, mage::Node* prevNode)
{
	gameChunks[crtNode->coord.first * WIDTH + crtNode->coord.second].setNumberOfNeighbors(crtNode->totNeighbors);
	if (crtNode->totNeighbors == 1) portals[numberOfPortals++] = { crtNode->coord.second, crtNode->coord.first };

	bool* walls = (bool*)calloc(4, sizeof(bool));

	crtNode->color = mage::GRAY;
	for (int i = 0; i < crtNode->totNeighbors; i++)
	{
		walls[0] = walls[0] || (crtNode->coord.first - crtNode->neighbors[i]->coord.first > 0);			// walls[0]: wall up
		walls[1] = walls[1] || (crtNode->coord.first - crtNode->neighbors[i]->coord.first < 0);			// walls[1]: wall down
		walls[2] = walls[2] || (crtNode->coord.second - crtNode->neighbors[i]->coord.second > 0);		// walls[2]: wall left
		walls[3] = walls[3] || (crtNode->coord.second - crtNode->neighbors[i]->coord.second < 0);		// walls[3]: wall right

		if (prevNode == nullptr || crtNode->neighbors[i] != prevNode)
		{
			if (crtNode->neighbors[i]->color != mage::BLACK)
			{
				return;
			}
			else
			{
				dfs_visit(crtNode->neighbors[i], crtNode);
			}
		}
	}

	gameChunks[crtNode->coord.first * WIDTH + crtNode->coord.second].setWalls(walls);
	crtNode->color = mage::WHITE;
}

void print_game_surface()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			for (int c = 0; c < 4; c++)
			{
				std::cout << gameChunks[i * WIDTH + j].getWalls()[c];
			}
			std::cout << " ";
		}
		std::cout << "\n";
	}
}

bool populate_game_surface()
{
	bool success = true;

	portals = (std::pair<int, int>*)calloc(WIDTH, sizeof(std::pair<int, int>));
	if (portals == NULL)
	{
		success = false;
		std::cerr << "Failed to allocate memory for portals!\n";
	}
	else
	{
		gameChunks = (GameChunk*)calloc(HEIGHT * WIDTH, sizeof(GameChunk));
		if (gameChunks == NULL)
		{
			success = false;
			std::cerr << "Failed to allocate memory for game surface!\n";
		}
		else
		{
			mage::Node* nodes = mage::fn_create(HEIGHT, WIDTH);
			if (nodes == NULL)
			{
				success = false;
				std::cerr << "Failed to fetch the generated maze!\n";
			}
			else
			{
				dfs_visit(nodes, NULL);
			}
		}
	}

	return success;
}

void convert_maze_elements()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			gameElements[i * WIDTH + j] = gameChunks[i * WIDTH + j].toGameElement({ j, i });
		}
	}

}

bool initialize_game()
{
	bool success = true;

	free(gameElements);
	srand((NULL));

	mainCharacter = PlayerCharacter(std::pair<int, int>{0, 0}, "./assets/demo.png");
	npCharacter = NonplayerCharacter(std::pair<int, int>{10, 1}, "./assets/knight_f_idle_anim_f0.png");


	populate_game_surface();

	gameElements = (GameElement*)calloc(WIDTH * HEIGHT + 2, sizeof(GameElement));	// first alloc memory for the map of the maze
	if (gameElements == NULL)
	{
		success = false;
		std::cerr << "Failed to alloc memory for gameElements!\n";
	}
	else
	{
		convert_maze_elements();

		gameElements[WIDTH * HEIGHT] = GameElement(mainCharacter.getCoordinatesInPixels(), mainCharacter.getDimensions(), mainCharacter.getAssetPath());
		gameElements[WIDTH * HEIGHT + 1] = GameElement(npCharacter.getCoordinatesInPixels(), npCharacter.getDimensions(), npCharacter.getAssetPath());

		print_game_surface();

	}

	return success;
}

bool initialize_titlescreen()
{
	bool success = true;

	free(gameElements);
	gameElements = (GameElement*)calloc(3, sizeof(GameElement));
	if (gameElements == NULL)
	{
		success = false;
		std::cerr << "Failed to alloc memory for gameElements!\n";
	}
	else
	{
		//gameElements[0] = titleButtons[0].toGameElement();
		//gameElements[1] = titleButtons[1].toGameElement();

	}

	return success;
}

bool initialize()
{
	bool success = true;

	if (!fn_initialize(&window, &renderer, &viewports, "Test game loop"))
	{
		std::cerr << "Error initializing the window!\n";
		success = false;
	}
	else
	{
		currentProgramState = GAME;
		previousProgramState = GAME;

		success = initialize_game();
	}

	return success;
}

void close()
{
	free(renderer);
	free(window);
	free(gameElements);
	free(gameChunks);
	free(portals);
}

int main(int argc, char* args[])
{
	//fn_demo();

	if (!initialize())
	{
		std::cerr << "Failed to initialize!\n";
	}
	else
	{
		//Main loop flag
		bool quit = false;

		int totGameELements = HEIGHT * WIDTH + 2;

		//Event handler
		SDL_Event e;

		while (!quit)
		{
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)	
				{
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					InputState inputState = getNextSubstate(e.key.keysym.sym);

					switch (currentProgramState)
					{
					case TITLE_SCREEN:
						if (previousProgramState != currentProgramState)
						{
							// initialize title_screen
							totGameELements = 3;
						}
						// process input
						break;
					case GAME:
						if (previousProgramState != GAME && previousProgramState != PAUSE_MENU )
						{
							initialize_game();
							totGameELements = WIDTH * HEIGHT + 2;
							timer = 0;
						}

						process_game_state(inputState, &mainCharacter, &npCharacter, gameChunks, portals, numberOfPortals);

						break;
					case PAUSE_MENU:
						if (previousProgramState != PAUSE_MENU && previousProgramState == GAME)
						{
							// initialize_ingame_menu();
							totGameELements += 3;
						}
						// process input
						break;
					case WIN_SCREEN:
						if (previousProgramState != currentProgramState && previousProgramState == GAME)
						{
							totGameELements = 3;
						}
						// process input
						break;
					case LOSE_SCREEN:
						if (previousProgramState != currentProgramState && previousProgramState == GAME)
						{
							totGameELements = 3;
						}
						// process input
						break;
					default:
						break;
					}

					previousProgramState = currentProgramState;
				}

				if (currentProgramState == GAME)
				{
					timer++;
					if (timer % 5 == 0)
					{
						timer = 0;
						if (mainCharacter.hasCooldown())
						{
							mainCharacter.decreaseCooldown();
						}

						if (npCharacter.getStunCooldownTimer() > 0)
						{
							npCharacter.decrementStunCooldownTimer();
						}
						else
						{
							process_npc_state(&npCharacter, gameChunks[npCharacter.getCoordinatesInGameChunks().second * WIDTH + npCharacter.getCoordinatesInGameChunks().first]);
						}
					}

					gameElements[WIDTH * HEIGHT] = GameElement(mainCharacter.getCoordinatesInPixels(), mainCharacter.getDimensions(), mainCharacter.getAssetPath());
					gameElements[WIDTH * HEIGHT + 1] = GameElement(npCharacter.getCoordinatesInPixels(), npCharacter.getDimensions(), npCharacter.getAssetPath());
				}

				fn_update(renderer, gameElements, viewports, 1, totGameELements);
			}


		}
	}

	//Free resources and close SDL
	//close();

	return 0;

}