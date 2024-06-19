#include<iostream>

#include "maze_gen/MaGe.h"
#include "StateProcessing.h"
#include "vis_lib/VisLib.h"
#include "vis_lib/GameElement.h"
#include "path_computing/PaCo.h"

#include "Utils.h"
#include "GameChunk.h"
#include "Character.h"

#include<SDL.h>
#undef main

SDL_Rect* viewports = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int timer = 0;

GameChunk gameChunks[135];
std::pair<int, int> portals[45];
int numberOfPortals = 0;

MainState currentProgramState;
MainState previousProgramState;

GameElement gameElements[HEIGHT * WIDTH + 5];
GameElement* auxElems = (GameElement*)calloc(3, sizeof(GameElement));
paco::DL_List* existingPath = NULL;


// ----- game views
Menu titleScreen = Menu(std::pair<int, int>{ VIEWPORT_WIDTH, VIEWPORT_HEIGHT }, std::pair<int, int>{40, 20}, std::string(MENUS_SRC_PATH"/titlescreen.png"));
Menu winScreen = Menu(std::pair<int, int>{ VIEWPORT_WIDTH, VIEWPORT_HEIGHT }, std::pair<int, int>{20, 30}, std::string(MENUS_SRC_PATH"/winscreen.png"));
Menu loseScreen = Menu(std::pair<int, int>{ VIEWPORT_WIDTH, VIEWPORT_HEIGHT }, std::pair<int, int>{20, 30}, std::string(MENUS_SRC_PATH"/losescreen.png"));
Menu ingameScreen = Menu(std::pair<int, int>{ 500, 300 }, std::pair<int, int>{290, 120}, std::string(MENUS_SRC_PATH"/ingamescreen.png"));

// ------ in-game elements
PlayerCharacter mainCharacter;
NonplayerCharacter npCharacter;


// ---------------------------------------------------- INIT GAME SURFACE

void dfs_visit(mage::Node* crtNode, mage::Node* prevNode)
{
	paco::Node *pathNode = gameChunks[crtNode->coord.first * WIDTH + crtNode->coord.second].getPathNode();
	pathNode->numberOfNeighbors = crtNode->totNeighbors;
	pathNode->neighbors = (paco::Node**)calloc(pathNode->numberOfNeighbors, sizeof(paco::Node*));

	gameChunks[crtNode->coord.first * WIDTH + crtNode->coord.second].setNumberOfNeighbors(crtNode->totNeighbors);

	if (crtNode->totNeighbors == 1 && crtNode->coord != std::pair<int,int>{HEIGHT-1, WIDTH-1})
	{
		if (numberOfPortals < 44)
		{
			portals[numberOfPortals] = { crtNode->coord.second, crtNode->coord.first };
			numberOfPortals++;
		}
	}

	bool* walls = (bool*)calloc(4, sizeof(bool));

	crtNode->color = mage::GRAY;
	for (int i = 0; i < crtNode->totNeighbors; i++)
	{
		walls[0] = walls[0] || (crtNode->coord.first - crtNode->neighbors[i]->coord.first > 0);			// walls[0]: wall up
		walls[1] = walls[1] || (crtNode->coord.first - crtNode->neighbors[i]->coord.first < 0);			// walls[1]: wall down
		walls[2] = walls[2] || (crtNode->coord.second - crtNode->neighbors[i]->coord.second > 0);		// walls[2]: wall left
		walls[3] = walls[3] || (crtNode->coord.second - crtNode->neighbors[i]->coord.second < 0);		// walls[3]: wall right

		pathNode->neighbors[i] = gameChunks[crtNode->neighbors[i]->coord.first * WIDTH + crtNode->neighbors[i]->coord.second].getPathNode();

		if (prevNode == nullptr || crtNode->neighbors[i] != prevNode)
		{
			if (crtNode->neighbors[i]->color != mage::BLACK)
			{
				continue;
			}
			else
			{
				dfs_visit(crtNode->neighbors[i], crtNode);
			}
		}
	}

	gameChunks[crtNode->coord.first * WIDTH + crtNode->coord.second].setPathNode(pathNode);
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

bool create_path_nodes()
{
	bool success = true;
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			paco::Node* pathNode = (paco::Node*)malloc(sizeof(paco::Node));
			if (pathNode)
			{
				pathNode->color = paco::WHITE;
				pathNode->coordinates = { j, i };

				gameChunks[i * WIDTH + j].setPathNode(pathNode);
			}
			else
			{
				std::cerr << "Failed to allocate memory for pathNode!\n";
				success = false;
				break;
			}
		}
	}

	return success;
}

bool populate_game_surface()
{
	bool success = true;

	numberOfPortals = 0;
	if (portals == nullptr)
	{
		success = false;
		std::cerr << "Failed to allocate memory for portals!\n";
	}
	else
	{
		mage::Node* nodes = mage::fn_create(HEIGHT, WIDTH);

		if (nodes == nullptr)
		{
			success = false;
			std::cerr << "Failed to fetch the generated maze!\n";
		}
		else
		{
			if (create_path_nodes())
			{
				dfs_visit(nodes, NULL);
			}
			else
			{
				success = false;
			}
		}

		mage::fn_clean(nodes, NULL);
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
	numberOfPortals = 0;
	timer = 0;
	existingPath = NULL;

	srand((NULL));

	mainCharacter.move(MOVE_RIGHT, { 0, 0 });
	npCharacter.move(MOVE_LEFT, { 14, 0 });

	populate_game_surface();

	convert_maze_elements();

	gameElements[WIDTH * HEIGHT] = GameElement(mainCharacter.getCoordinatesInPixels(), mainCharacter.getDimensions(), mainCharacter.getAssetPath());
	gameElements[WIDTH * HEIGHT + 1] = GameElement(npCharacter.getCoordinatesInPixels(), npCharacter.getDimensions(), npCharacter.getAssetPath());

	return success;
}

MainState async_game_updates()
{
	MainState currentMainState = GAME_STATE;
	std::pair<int, int> crtNpcCoords = npCharacter.getCoordinatesInGameChunks();
	std::pair<int, int> crtMainChrCoords = mainCharacter.getCoordinatesInGameChunks();

	timer++;
	if (timer % 5 == 0)
	{
		std::cout << "timer: " << timer << "\n";
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
			if (crtNpcCoords == crtMainChrCoords)
			{
				return LOSE_STATE;
			}
			existingPath = process_npc_state(&npCharacter, 
				gameChunks[crtMainChrCoords.second * WIDTH + crtMainChrCoords.first].getPathNode(),
				gameChunks[crtNpcCoords.second * WIDTH + crtNpcCoords.first],
				existingPath);
		}

		if (timer == 1000)
		{
			std::cout << "special event trigger\n";
			timer = 0;
		}
	}

	gameElements[WIDTH * HEIGHT + 1] = GameElement(npCharacter.getCoordinatesInPixels(), npCharacter.getDimensions(), npCharacter.getAssetPath());

	if (crtMainChrCoords.first == WIDTH - 1 && crtMainChrCoords.second == HEIGHT - 1)
	{
		currentMainState = WIN_STATE;
	}
	else if (crtMainChrCoords == crtNpcCoords && npCharacter.getStunCooldownTimer() == 0)
	{
		currentMainState = LOSE_STATE;
	}

	return currentMainState;
}

bool initialize_winscreen()
{
	bool success = true;

	numberOfPortals = 0;

	gameElements[0] = winScreen.toGameElements()[0];
	gameElements[1] = winScreen.toGameElements()[1];
	gameElements[2] = winScreen.toGameElements()[2];

	return success;
}

bool initialize_loseScreen()
{
	bool success = true;

	numberOfPortals = 0;

	gameElements[0] = loseScreen.toGameElements()[0];
	gameElements[1] = loseScreen.toGameElements()[1];
	gameElements[2] = loseScreen.toGameElements()[2];

	return success;
}

bool initialize_ingame_menu()
{
	bool success = true;

	auxElems = ingameScreen.toGameElements();
	for (int i = 0; i < 3; i++)
	{
		gameElements[WIDTH * HEIGHT + 2 + i] = auxElems[i];
	}

	return success;
}

bool initialize_titlescreen()
{
	bool success = true;

	gameElements[0] = titleScreen.toGameElements()[0];
	gameElements[1] = titleScreen.toGameElements()[1];
	gameElements[2] = titleScreen.toGameElements()[2];

	return success;
}

int initialize_new_state(MainState mainState)
{
	int numberOfElements = 0;
	switch (mainState)
	{
	case TITLE_STATE:
		initialize_titlescreen();
		numberOfElements = 3;
		break;
	case GAME_STATE:
		initialize_game();
		numberOfElements = HEIGHT * WIDTH + 2;
		break;
	case WIN_STATE:
		initialize_winscreen();
		numberOfElements = 3;
		break;
	case LOSE_STATE:
		initialize_loseScreen();
		numberOfElements = 3;
		break;
	case PAUSE_STATE:
		initialize_ingame_menu();
		numberOfElements = HEIGHT * WIDTH + 5;
		break;
	default:
		break;
	}

	return numberOfElements;
}

bool initialize()
{
	bool success = true;
	viewports = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (viewports == NULL)
	{
		std::cerr << "Error initializing the window!\n";
		exit(-1);
	}
	viewports->x = 40;
	viewports->y = 30;
	viewports->w = 1000;
	viewports->h = 600;

	if (!fn_initialize(&window, &renderer, "Demo"))
	{
		std::cerr << "Error initializing the window!\n";
		success = false;
	}
	else
	{
		Button* buttonsEndscreen = (Button*)calloc(2, sizeof(Button));
		if (buttonsEndscreen)
		{
			buttonsEndscreen[0] = Button(std::pair<int, int>{400, 448}, std::string(MENUS_SRC_PATH"/newgame"), GAME_STATE);
			buttonsEndscreen[1] = Button(std::pair<int, int>{600, 448}, std::string(MENUS_SRC_PATH"/backtomenu"), TITLE_STATE);
			loseScreen.setButtons(buttonsEndscreen, 2);
			winScreen.setButtons(buttonsEndscreen, 2);
		}

		Button* buttonsIngame = (Button*)calloc(2, sizeof(Button));
		if (buttonsIngame)
		{
			buttonsIngame[0] = Button(std::pair<int, int>{400, 348}, std::string(MENUS_SRC_PATH"/backtogame"), GAME_STATE);
			buttonsIngame[1] = Button(std::pair<int, int>{600, 348}, std::string(MENUS_SRC_PATH"/backtomenu"), TITLE_STATE);
			ingameScreen.setButtons(buttonsIngame, 2);
		}

		Button* buttonsTitle = (Button*)calloc(2, sizeof(Button));
		if (buttonsTitle)
		{
			buttonsTitle[0] = Button(std::pair<int, int>{476, 352}, std::string(MENUS_SRC_PATH"/newgame"), GAME_STATE);
			buttonsTitle[1] = Button(std::pair<int, int>{476, 448}, std::string(MENUS_SRC_PATH"/exit"), EXIT_STATE);
			titleScreen.setButtons(buttonsTitle, 2);
		}

		mainCharacter = PlayerCharacter(std::pair<int, int>{0, 0}, "./assets/demo.png");
		npCharacter = NonplayerCharacter(std::pair<int, int>{10, 1}, "./assets/knight_f_idle_anim_f0.png");

		currentProgramState = previousProgramState = TITLE_STATE;

		success = initialize_titlescreen();
	}

	return success;
}

void close()
{
	//free(gameElements);
	//free(gameChunks);
	//free(portals);
}

int main(int argc, char* args[])
{
	if (!initialize())
	{
		std::cerr << "Failed to initialize!\n";
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//int numberOfGameElements = HEIGHT * WIDTH + 2;
		int numberOfGameElements = 3;

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
					MainState nextProgramState = SAME_STATE;
					switch (currentProgramState)
					{
					case TITLE_STATE:
						nextProgramState = process_menu_state(inputState, &titleScreen);
						gameElements[0] = titleScreen.toGameElements()[0];
						gameElements[1] = titleScreen.toGameElements()[1];
						gameElements[2] = titleScreen.toGameElements()[2];
						break;

					case GAME_STATE:						
						nextProgramState = process_game_state(inputState, &mainCharacter, &npCharacter, gameChunks, portals, numberOfPortals);
						gameElements[WIDTH * HEIGHT] = GameElement(mainCharacter.getCoordinatesInPixels(), mainCharacter.getDimensions(), mainCharacter.getAssetPath());
						break;

					case PAUSE_STATE:
						nextProgramState = process_menu_state(inputState, &ingameScreen);
						auxElems = ingameScreen.toGameElements();
						for (int i = 0; i < 3; i++)
						{
							gameElements[WIDTH * HEIGHT + 2 + i] = auxElems[i];
						}
						break;

					case WIN_STATE:
						nextProgramState = process_menu_state(inputState, &winScreen);
						gameElements[0] = winScreen.toGameElements()[0];
						gameElements[1] = winScreen.toGameElements()[1];
						gameElements[2] = winScreen.toGameElements()[2];
						break;

					case LOSE_STATE:
						nextProgramState = process_menu_state(inputState, &loseScreen);
						gameElements[0] = loseScreen.toGameElements()[0];
						gameElements[1] = loseScreen.toGameElements()[1];
						gameElements[2] = loseScreen.toGameElements()[2];
						break;

					default:
						nextProgramState = EXIT_STATE;
						break;
					}
					
					previousProgramState = currentProgramState;
					if (nextProgramState != SAME_STATE || (currentProgramState == GAME_STATE && nextProgramState == PAUSE_STATE))
					{
						currentProgramState = nextProgramState;
					}
				}
			}

			if (currentProgramState == previousProgramState && currentProgramState == GAME_STATE)
			{
				currentProgramState = async_game_updates();
			}

			if (previousProgramState != currentProgramState)
			{
				if (previousProgramState == PAUSE_STATE && currentProgramState == GAME_STATE)
				{
					numberOfGameElements = 137;
				}
				else
				{
					numberOfGameElements = initialize_new_state(currentProgramState);
				}
				previousProgramState = currentProgramState;
			}

			if (currentProgramState == EXIT_STATE)
			{
				quit = true;
			}


			if (quit == false)
			{
				fn_update(renderer, gameElements, viewports, 1, numberOfGameElements);
			}
		}
	}

	fn_clean(window, renderer, viewports);

	return 0;

}