#include "StateProcessing.h"

// ------------------------------------------------------------------------------------------------------- CHARACTER INPUT

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

bool check_inbounds(CharacterState playerState, std::pair<int, int> playerCoordinates)
{
	bool isInbounds = (playerState != MOVE_UP || playerCoordinates.second != 0);
	isInbounds = isInbounds && (playerState != MOVE_DOWN || playerCoordinates.second != HEIGHT - 1);
	isInbounds = isInbounds && (playerState != MOVE_LEFT || playerCoordinates.first != 0);
	isInbounds = isInbounds && (playerState != MOVE_RIGHT || playerCoordinates.first != WIDTH - 1);

	return isInbounds;
}

std::pair<int, int> movement(CharacterState characterState, std::pair<int, int> characterCoordinates, bool crtWalls[])
{
	std::pair<int, int> newCoordinates = characterCoordinates;

	std::cout << "walls for crt node " << characterCoordinates.first << " " << characterCoordinates.second << " ";
	for (int i = 0; i < 4; i++) {
		std::cout << crtWalls[i];
	}
	std::cout << "\n";

	if (check_inbounds(characterState, newCoordinates))
	{
		switch (characterState)
		{
		case MOVE_UP:
			if (crtWalls[0]) newCoordinates.second -= 1;
			break;
		case MOVE_DOWN:
			if (crtWalls[1]) newCoordinates.second += 1;
			break;
		case MOVE_LEFT:
			if (crtWalls[2]) newCoordinates.first -= 1;
			break;
		case MOVE_RIGHT:
			if (crtWalls[3]) newCoordinates.first += 1;
			break;
		default:
			break;
		}
	}

	return newCoordinates;
}

void process_player_state(PlayerCharacter* mainCharacter, NonplayerCharacter* npc, InputState inputState, GameChunk crtGameChunk, std::pair<int, int>* portals, int numberOfPortals)
{
	CharacterState nextState = convert_input_to_player_state(inputState);
	switch (nextState)
	{
	case MOVE_UP:
	case MOVE_DOWN:
	case MOVE_LEFT:
	case MOVE_RIGHT:
		mainCharacter->move(nextState, movement(nextState, mainCharacter->getCoordinatesInGameChunks(),
			crtGameChunk.getWalls()));
		break;
	case INTERACT:
		if (crtGameChunk.getNumberOfNeighbors() == 1)	mainCharacter->move(MOVE_UP, portals[rand() % numberOfPortals-1]);
		break;
	case ATTACK:
		// attack will be added
		break;
	default:
		nextState = IDLE;
	}
}

void process_npc_state(NonplayerCharacter* npc, PlayerCharacter* playerCharacter, GameChunk crtGameChunk)
{
	// use timer
	if (npc->getStunCooldownTimer() > 0)
	{
		npc->decrementStunCooldownTimer();
	}
	else
	{
		// move
	}
}

// --------------------------------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------- GAME SCREEN INPUT

MainState process_game_state(InputState inputState, PlayerCharacter* mainCharacter, NonplayerCharacter* npc, GameChunk* gameChunks, std::pair<int, int>* portals, int numberOfPortals)
{
	MainState nextMainState = NO_CHANGE;
	switch (inputState)
	{
	case PAUSE:
		nextMainState = PAUSE_MENU;
		break;
	default:
		process_player_state(mainCharacter, npc, inputState, gameChunks[mainCharacter->getCoordinatesInGameChunks().second * WIDTH + mainCharacter->getCoordinatesInGameChunks().first], portals, numberOfPortals);
		// process npc
		if (mainCharacter->getCoordinatesInGameChunks() == std::pair<int, int>(WIDTH, HEIGHT))
		{
			nextMainState = WIN_SCREEN;
		}
		else if (mainCharacter->getCoordinatesInGameChunks() == npc->getCoordinatesInGameChunks() 
					&& npc->getStunCooldownTimer() == 0)
		{
			nextMainState = LOSE_SCREEN;
		}
	}

	return nextMainState;
}


// --------------------------------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------- MENU SCREENS INPUT


MainState process_menu_state(InputState inputState, Menu* crtMenu)
{
	MainState nextMainState = NO_CHANGE;
	switch (inputState)
	{
	case ARROW_DOWN:
	case ARROW_UP:
	case ARROW_LEFT:
	case ARROW_RIGHT:
		crtMenu->changeActiveButton();
		break;
	case ATTACK_INPUT:
		nextMainState = crtMenu->getActiveButton().triggerEvent();
		break;
	default:
		break;
	}

	return nextMainState;
}
