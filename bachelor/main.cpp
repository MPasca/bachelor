#include<iostream>

#include "maze_gen/MaGe.h"
#include "StateProcessing.h"
#include "vis_lib/VisLib.h"
#include "vis_lib/GameElement.h"
#include "path_computing/PaCo.h"

#include "Utils.h"
#include "GameChunk.h"
#include "Character.h"
#include "GameProp.h"

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

GameElement gameElements[HEIGHT * WIDTH + 7];
GameElement* auxElems = (GameElement*)calloc(3, sizeof(GameElement));
paco::DL_List* existingPath = NULL;


// ----- game views
Menu titleScreen = Menu(std::pair<int, int>{ VIEWPORT_WIDTH, VIEWPORT_HEIGHT }, std::pair<int, int>{0, 0}, std::string(MENUS_SRC_PATH"/titlescreen.png"));
Menu winScreen = Menu(std::pair<int, int>{ VIEWPORT_WIDTH, VIEWPORT_HEIGHT }, std::pair<int, int>{0, 0}, std::string(MENUS_SRC_PATH"/winscreen.png"));
Menu loseScreen = Menu(std::pair<int, int>{ VIEWPORT_WIDTH, VIEWPORT_HEIGHT }, std::pair<int, int>{0, 0}, std::string(MENUS_SRC_PATH"/losescreen.png"));
Menu ingameScreen = Menu(std::pair<int, int>{ 500, 300 }, std::pair<int, int>{290, 120}, std::string(MENUS_SRC_PATH"/ingamescreen.png"));

// ------ in-game elements
PlayerCharacter mainCharacter;
NonplayerCharacter npCharacter;
GameProp exitKey;
GameProp teleportAbility;

// ----------------------------------- Functions

MainState async_game_updates()
{
	std::pair<int, int> crtNpcCoords = npCharacter.getCoordinatesInGameChunks();
	std::pair<int, int> crtMainChrCoords = mainCharacter.getCoordinatesInGameChunks();

	if (exitKey.isConsumed())
	{
		gameElements[WIDTH * HEIGHT + 2].setPath(PROP_PATH"/blank.png");
	}

	if (teleportAbility.isConsumed())
	{
		gameElements[WIDTH * HEIGHT + 3].setPath(PROP_PATH"/blank.png");
	}

	if (mainCharacter.hasKey() && crtMainChrCoords == std::pair<int, int>{WIDTH - 1, HEIGHT - 1})
	{
		return WIN_STATE;
	}


	timer++;
	if (timer % 8 == 0)
	{
		std::cout << "timer: " << timer << "\n";
		if (mainCharacter.hasAttackCooldown())
		{
			mainCharacter.decreaseAttackCooldown();
		}

		if (mainCharacter.hasTeleportCooldown())
		{
			mainCharacter.decreaseTeleportCooldown();
		}

		if (npCharacter.getStunCooldownTimer() > 0)
		{	
			npCharacter.decrementStunCooldownTimer();
		}
		else
		{
			if (crtMainChrCoords == crtNpcCoords)
			{
				return LOSE_STATE;
			}
			existingPath = stateprocessing::process_npc_state(&npCharacter,
				gameChunks[crtMainChrCoords.second * WIDTH + crtMainChrCoords.first].getPathNode(),
				gameChunks[crtNpcCoords.second * WIDTH + crtNpcCoords.first],
				existingPath);
		}

		if (timer == 80 && !teleportAbility.isConsumed())
		{
			teleportAbility.reveal();
			gameElements[WIDTH * HEIGHT + 3].setPath(PROP_PATH"/teleport.png");
		}

		if (timer == 120 && !exitKey.isConsumed())
		{
			exitKey.reveal();
			gameElements[WIDTH * HEIGHT + 2].setPath(PROP_PATH"/key.png");
		}
	}

	gameElements[WIDTH * HEIGHT + 1].updateCoordinates(npCharacter.getCoordinatesInPixels());
	gameElements[WIDTH * HEIGHT + 1].setPath(npCharacter.getAssetPath());

	return GAME_STATE;
}

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

	bool* neighbors = (bool*)calloc(4, sizeof(bool));

	crtNode->color = mage::GRAY;
	for (int i = 0; i < crtNode->totNeighbors; i++)
	{
		neighbors[0] = neighbors[0] || (crtNode->coord.first - crtNode->neighbors[i]->coord.first > 0);			// neighbors[0]: wall up
		neighbors[1] = neighbors[1] || (crtNode->coord.first - crtNode->neighbors[i]->coord.first < 0);			// neighbors[1]: wall down
		neighbors[2] = neighbors[2] || (crtNode->coord.second - crtNode->neighbors[i]->coord.second > 0);		// neighbors[2]: wall left
		neighbors[3] = neighbors[3] || (crtNode->coord.second - crtNode->neighbors[i]->coord.second < 0);		// neighbors[3]: wall right

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
	gameChunks[crtNode->coord.first * WIDTH + crtNode->coord.second].setNeighbors(neighbors);
	gameChunks[crtNode->coord.first * WIDTH + crtNode->coord.second].setCoordinates({crtNode->coord.second, crtNode->coord.first});
	crtNode->color = mage::WHITE;
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
			gameElements[i * WIDTH + j] = gameChunks[i * WIDTH + j].toGameElement();
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
	mainCharacter.resetCharacter();
	npCharacter.move(MOVE_LEFT, { 14, 0 });

	exitKey.resetProp();
	teleportAbility.resetProp();

	std::pair<int, int> auxCoordinates;
	do {
		auxCoordinates.first = rand() % (WIDTH - 1);
		auxCoordinates.second = rand() % (HEIGHT - 1);
	} while (auxCoordinates == std::pair<int, int>{0, 0} || auxCoordinates == std::pair<int, int>{WIDTH - 1, HEIGHT - 1});

	exitKey.updateCoordinates(auxCoordinates);
	do {
		auxCoordinates.first = rand() % (WIDTH - 1);
		auxCoordinates.second = rand() % (HEIGHT - 1);
	} while (auxCoordinates == std::pair<int, int>{0, 0} || auxCoordinates == std::pair<int, int>{WIDTH - 1, HEIGHT - 1});

	teleportAbility.updateCoordinates(auxCoordinates);


	success = populate_game_surface();

	convert_maze_elements();

	gameElements[WIDTH * HEIGHT] = GameElement(mainCharacter.getCoordinatesInPixels(), mainCharacter.getDimensions(), mainCharacter.getAssetPath());
	gameElements[WIDTH * HEIGHT + 1] = GameElement(npCharacter.getCoordinatesInPixels(), npCharacter.getDimensions(), npCharacter.getAssetPath());
	gameElements[WIDTH * HEIGHT + 2] = GameElement(exitKey.getCoordinatesInPixels(), exitKey.getDimensions(), exitKey.getAssetPath());
	gameElements[WIDTH * HEIGHT + 3] = GameElement(teleportAbility.getCoordinatesInPixels(), teleportAbility.getDimensions(), teleportAbility.getAssetPath());

	return success;
}

bool initialize_menu(Menu crtMenu, int numberOfElements)
{
	bool success = true;

	auxElems = crtMenu.toGameElements();
	for (int i = 0; i < 3; i++)
	{
		gameElements[numberOfElements + i] = auxElems[i];
	}

	return success;
}

int initialize_new_state(MainState mainState)
{
	switch (mainState)
	{
	case TITLE_STATE:
		initialize_menu(titleScreen, 0);
		return 3;
	case GAME_STATE:
		initialize_game();
		return HEIGHT * WIDTH + 4;
	case WIN_STATE:
		initialize_menu(winScreen, 0);
		return 3;
	case LOSE_STATE:
		initialize_menu(loseScreen, 0);
		return 3;
	case PAUSE_STATE:
		initialize_menu(ingameScreen, HEIGHT * WIDTH + 4);
		return HEIGHT * WIDTH + 7;
	default:
		return 0;
	}
}

bool fn_initialize()
{
	bool success = true;
	viewports = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (viewports == NULL)
	{
		std::cerr << "Error initializing the window!\n";
		exit(-1);
	}
	viewports->x = 40;
	viewports->y = 20;
	viewports->w = 1000;
	viewports->h = 600;

	if (!vislib::fn_initialize(&window, &renderer, "Demo"))
	{
		std::cerr << "Error initializing the window!\n";
		success = false;
	}
	else
	{
		Button* buttonsEndscreen = (Button*)calloc(2, sizeof(Button));
		if (buttonsEndscreen)
		{
			buttonsEndscreen[0] = Button(std::pair<int, int>{320, 448}, std::string(MENUS_SRC_PATH"/newgame"), GAME_STATE);
			buttonsEndscreen[1] = Button(std::pair<int, int>{520, 448}, std::string(MENUS_SRC_PATH"/backtomenu"), TITLE_STATE);
			loseScreen.setButtons(buttonsEndscreen, 2);
			winScreen.setButtons(buttonsEndscreen, 2);
		}

		Button* buttonsIngame = (Button*)calloc(2, sizeof(Button));
		if (buttonsIngame)
		{
			buttonsIngame[0] = Button(std::pair<int, int>{400, 328}, std::string(MENUS_SRC_PATH"/backtogame"), GAME_STATE);
			buttonsIngame[1] = Button(std::pair<int, int>{600, 328}, std::string(MENUS_SRC_PATH"/backtomenu"), TITLE_STATE);
			ingameScreen.setButtons(buttonsIngame, 2);
		}

		Button* buttonsTitle = (Button*)calloc(2, sizeof(Button));
		if (buttonsTitle)
		{
			buttonsTitle[0] = Button(std::pair<int, int>{436, 352}, std::string(MENUS_SRC_PATH"/newgame"), GAME_STATE);
			buttonsTitle[1] = Button(std::pair<int, int>{436, 448}, std::string(MENUS_SRC_PATH"/exit"), EXIT_STATE);
			titleScreen.setButtons(buttonsTitle, 2);
		}

		mainCharacter = PlayerCharacter(std::pair<int, int>{0, 0}, PLAYERCHR_PATH);
		npCharacter = NonplayerCharacter(std::pair<int, int>{10, 1}, NPC_PATH);

		std::pair<int, int> auxCoordinates;
		do {
			auxCoordinates.first = rand() % (WIDTH - 1);
			auxCoordinates.second = rand() % (HEIGHT - 1);
		} while (auxCoordinates == std::pair<int, int>{0, 0} || auxCoordinates == std::pair<int, int>{WIDTH - 1, HEIGHT - 1});

		exitKey = GameProp(auxCoordinates, std::string(PROP_PATH"/key.png"));
		do {
			auxCoordinates.first = rand() % (WIDTH - 1);
			auxCoordinates.second = rand() % (HEIGHT - 1);
		} while (auxCoordinates == std::pair<int, int>{0, 0} || auxCoordinates == std::pair<int, int>{WIDTH - 1, HEIGHT - 1});

		teleportAbility = GameProp(auxCoordinates, std::string(PROP_PATH"/teleport.png"));

		currentProgramState = previousProgramState = TITLE_STATE;

		success = initialize_menu(titleScreen, 0);
	}

	return success;
}

void print_game_surface()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			for (int c = 0; c < 4; c++)
			{
				std::cout << gameChunks[i * WIDTH + j].getNeighbors()[c];
			}
			std::cout << " ";
		}
		std::cout << "\n";
	}
}

int main(int argc, char* args[])
{
	if (!fn_initialize())
	{
		std::cerr << "Failed to initialize!\n";
	}
	else
	{
		//Main loop flag
		bool quit = false;

		int numberOfGameElements = 3;

		//Event handler
		SDL_Event e;

		while (!quit)
		{
			vislib::fn_update(renderer, gameElements, viewports, numberOfGameElements);

			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)	
				{
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					inputdcd::InputState inputState = inputdcd::convert_from_keyboard(e.key.keysym.sym);
					MainState nextProgramState = SAME_STATE;
					switch (currentProgramState)
					{
					case TITLE_STATE:
						nextProgramState = stateprocessing::process_menu_state(inputState, &titleScreen);
						auxElems = titleScreen.toGameElements();
						for (int i = 0; i < 3; i++)
						{
							gameElements[i] = auxElems[i];
						}
						break;

					case GAME_STATE:						
						nextProgramState = stateprocessing::process_game_state(inputState, &mainCharacter, &exitKey, &teleportAbility, &npCharacter, gameChunks, portals, numberOfPortals);
						gameElements[WIDTH * HEIGHT].updateCoordinates(mainCharacter.getCoordinatesInPixels());
						gameElements[WIDTH * HEIGHT].setPath(mainCharacter.getAssetPath());
						break;

					case PAUSE_STATE:
						nextProgramState = stateprocessing::process_menu_state(inputState, &ingameScreen);
						auxElems = ingameScreen.toGameElements();
						for (int i = 0; i < 3; i++)
						{
							gameElements[WIDTH * HEIGHT + 4 + i] = auxElems[i];
						}
						break;

					case WIN_STATE:
						nextProgramState = stateprocessing::process_menu_state(inputState, &winScreen);
						auxElems = winScreen.toGameElements();
						for (int i = 0; i < 3; i++)
						{
							gameElements[i] = auxElems[i];
						}
						break;

					case LOSE_STATE:
						nextProgramState = stateprocessing::process_menu_state(inputState, &loseScreen);
						auxElems = loseScreen.toGameElements();
						for (int i = 0; i < 3; i++)
						{
							gameElements[i] = auxElems[i];
						}
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

			if (currentProgramState == GAME_STATE && currentProgramState == previousProgramState)
			{
				currentProgramState = async_game_updates();
			}

			if (previousProgramState != currentProgramState && currentProgramState != EXIT_STATE)
			{
				if (previousProgramState == PAUSE_STATE && currentProgramState == GAME_STATE)
				{
					numberOfGameElements = 139;
				}
				else
				{
					numberOfGameElements = initialize_new_state(currentProgramState);
				}
				previousProgramState = currentProgramState;
			}				

			quit = quit || currentProgramState == EXIT_STATE;
		}

		vislib::fn_clean(window, renderer, viewports);
	}

	return 0;
}