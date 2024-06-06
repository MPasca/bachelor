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
GameElement* auxElems = (GameElement*)calloc(3, sizeof(GameElement));



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
	paco::Node *pathNodes = (paco::Node*)malloc(sizeof(paco::Node));
	pathNodes->color = paco::WHITE;
	pathNodes->coordinates = crtNode->coord;
	pathNodes->numberOfNeighbors = crtNode->totNeighbors;
	pathNodes->neighbors = (paco::Node**)calloc(pathNodes->numberOfNeighbors, sizeof(paco::Node*));

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
				pathNodes->neighbors[i] = gameChunks[crtNode->neighbors[i]->coord.first * WIDTH + crtNode->neighbors[i]->coord.second].getPathNode();
			}
		}
	}

	gameChunks[crtNode->coord.first * WIDTH + crtNode->coord.second].setPathNode(pathNodes);
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

	numberOfPortals = 0;
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
	numberOfPortals = 0;

	free(gameElements);
	srand((NULL));

	free(gameChunks);
	gameChunks = (GameChunk*)calloc(HEIGHT * WIDTH, sizeof(GameChunk));
	if (gameChunks)
	{
		mainCharacter.move(MOVE_RIGHT, { 0, 0 });
		npCharacter.move(MOVE_LEFT, { 14, 0 });

		populate_game_surface();

		gameElements = (GameElement*)calloc(WIDTH * HEIGHT + 2, sizeof(GameElement));
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
	}

	return success;
}

MainState async_game_updates()
{
	MainState currentMainState = GAME_STATE;
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
			//process_npc_state(&npCharacter, 
			//	gameChunks[mainCharacter.getCoordinatesInGameChunks().second * WIDTH + mainCharacter.getCoordinatesInGameChunks().first].getPathNode(),
			//	gameChunks[npCharacter.getCoordinatesInGameChunks().second * WIDTH + npCharacter.getCoordinatesInGameChunks().first]);
		}
	}

	//gameElements[WIDTH * HEIGHT + 1] = GameElement(npCharacter.getCoordinatesInPixels(), npCharacter.getDimensions(), npCharacter.getAssetPath());

	if (mainCharacter.getCoordinatesInGameChunks() == std::pair<int, int>(WIDTH - 1, HEIGHT - 1))
	{
		std::cout << "You win!" << "\n";
		currentMainState = WIN_STATE;
	}
	else if (mainCharacter.getCoordinatesInGameChunks() == npCharacter.getCoordinatesInGameChunks() && npCharacter.getStunCooldownTimer() == 0)
	{
		std::cout << "You lose" << "\n";
		currentMainState = LOSE_STATE;
	}

	return currentMainState;
}

bool initialize_winscreen()
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
		gameElements = winScreen.toGameElements();
	}

	return success;
}

bool initialize_loseScreen()
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
		gameElements = loseScreen.toGameElements();
	}

	return success;
}

bool initialize_ingame_menu()
{
	bool success = true;

	gameElements = (GameElement*)calloc(WIDTH * HEIGHT + 5, sizeof(GameElement));
	if (gameElements == NULL)
	{
		success = false;
		std::cerr << "Failed to alloc memory for gameElements!\n";
	}
	else
	{
		auxElems = ingameScreen.toGameElements();
		for (int i = 0; i < 3; i++)
		{
			std::cout << auxElems[i].getPath() << " : {" << auxElems[i].getCoordinates().first << ", " << auxElems[i].getCoordinates().second << "}\n";
			gameElements[WIDTH * HEIGHT + 2 + i] = auxElems[i];
		}
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
		gameElements = titleScreen.toGameElements();
	}

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
		numberOfElements = HEIGHT * WIDTH + 4;
		std::cout << numberOfElements << "\n";
		break;
	default:
		break;
	}

	return numberOfElements;
}

bool initialize()
{
	bool success = true;

	if (!fn_initialize(&window, &renderer, &viewports, "Demo"))
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
			buttonsIngame[0] = Button(std::pair<int, int>{400, 448}, std::string(MENUS_SRC_PATH"/backtogame"), GAME_STATE);
			buttonsIngame[1] = Button(std::pair<int, int>{600, 448}, std::string(MENUS_SRC_PATH"/backtomenu"), TITLE_STATE);
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
	free(gameElements);
	free(gameChunks);
	free(portals);
	free(viewports);
	free(renderer);
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
						std::cout << "nextProgramState: " << nextProgramState << "\n";
						gameElements = titleScreen.toGameElements();
						break;

					case GAME_STATE:						
						nextProgramState = process_game_state(inputState, &mainCharacter, &npCharacter, gameChunks, portals, numberOfPortals);
						gameElements[WIDTH * HEIGHT] = GameElement(mainCharacter.getCoordinatesInPixels(), mainCharacter.getDimensions(), mainCharacter.getAssetPath());
						//gameElements[WIDTH * HEIGHT].updateCoordinates(mainCharacter.getCoordinatesInPixels());
						std::cout << "gameElemsCoordinates: " << gameElements[WIDTH * HEIGHT].getCoordinates().first << " " << gameElements[WIDTH * HEIGHT].getCoordinates().second << "\n";
						break;

					case PAUSE_STATE:
						process_menu_state(inputState, &ingameScreen);
						auxElems = ingameScreen.toGameElements();
						for (int i = 0; i < 3; i++)
						{
							gameElements[WIDTH * HEIGHT + 2 + i] = auxElems[i];
						}
						break;

					case WIN_STATE:
						nextProgramState = process_menu_state(inputState, &winScreen);
						gameElements = winScreen.toGameElements();
						break;

					case LOSE_STATE:
						nextProgramState = process_menu_state(inputState, &loseScreen);
						gameElements = loseScreen.toGameElements();
						break;

					default:
						nextProgramState = EXIT_STATE;
						break;
					}
					
					previousProgramState = currentProgramState;
					if (nextProgramState != SAME_STATE && currentProgramState != GAME_STATE)
					{
						currentProgramState = nextProgramState;
					}
				}

				if (previousProgramState != currentProgramState)
				{
					numberOfGameElements = initialize_new_state(currentProgramState);
					previousProgramState = currentProgramState;
				}

				if (currentProgramState == EXIT_STATE)
				{
					quit = true;
				}
				else
				{
					if (currentProgramState == GAME_STATE)
					{
						currentProgramState = async_game_updates();
					}
					fn_update(renderer, gameElements, viewports, 1, numberOfGameElements);
				}
			}
		}
	}

	//Free resources and close SDL
	//close();

	return 0;

}