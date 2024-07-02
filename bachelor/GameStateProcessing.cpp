#include "GameStateProcessing.h"
#include "path_computing/PaCo.h"

using namespace inputdcd;

namespace stateprocessing
{
	CharacterState convert_input_to_player_state(InputState inputState)
	{
		switch (inputState)
		{
		case ARROW_DOWN:
			return MOVE_DOWN;
		case ARROW_UP:
			return MOVE_UP;
		case ARROW_LEFT:
			return MOVE_LEFT;
		case ARROW_RIGHT:
			return MOVE_RIGHT;
		case INTERACT_INPUT:
			return INTERACT;
		case ATTACK_INPUT:
			return ATTACK;
		default:
			return IDLE;
		}
	}

	bool check_inbounds(CharacterState playerState, std::pair<int, int> playerCoordinates)
	{
		bool isInbounds = (playerState != MOVE_UP || playerCoordinates.second != 0);
		isInbounds = isInbounds && (playerState != MOVE_DOWN || playerCoordinates.second != HEIGHT - 1);
		isInbounds = isInbounds && (playerState != MOVE_LEFT || playerCoordinates.first != 0);
		isInbounds = isInbounds && (playerState != MOVE_RIGHT || playerCoordinates.first != WIDTH - 1);

		return isInbounds;
	}

	std::pair<int, int> movement(CharacterState characterState, std::pair<int, int> characterCoordinates, bool crtNeighbors[])
	{
		if (characterCoordinates.first < 0 || characterCoordinates.first >= WIDTH
			|| characterCoordinates.second < 0 || characterCoordinates.second >= HEIGHT)
		{
			std::cerr << "Character coordinates out of bound!\n";
			exit(ERR_COORDINATES_OUT_OF_BOUND);
		}

		std::pair<int, int> newCoordinates = characterCoordinates;

		if (check_inbounds(characterState, newCoordinates))
		{
			switch (characterState)
			{
			case MOVE_UP:
				if (crtNeighbors[0]) newCoordinates.second -= 1;
				break;
			case MOVE_DOWN:
				if (crtNeighbors[1]) newCoordinates.second += 1;
				break;
			case MOVE_LEFT:
				if (crtNeighbors[2]) newCoordinates.first -= 1;
				break;
			case MOVE_RIGHT:
				if (crtNeighbors[3]) newCoordinates.first += 1;
				break;
			default:
				break;
			}
		}

		return newCoordinates;
	}

	MainState process_player_state(PlayerCharacter* mainCharacter, GameProp* key, GameProp* teleportAbility,  NonplayerCharacter* npCharacter, InputState inputState, GameChunk crtGameChunk, std::pair<int, int>* portals, int numberOfPortals)
	{
		if (npCharacter->getStunCooldownTimer() == 0 && mainCharacter->getCoordinatesInGameChunks() == npCharacter->getCoordinatesInGameChunks())
		{
			return LOSE_STATE;
		}
		CharacterState nextState = convert_input_to_player_state(inputState);
		std::pair<int, int> newMainChrCoordinates;
		switch (nextState)
		{
		case MOVE_UP:
		case MOVE_DOWN:
		case MOVE_LEFT:
		case MOVE_RIGHT:
			newMainChrCoordinates = movement(nextState, mainCharacter->getCoordinatesInGameChunks(), crtGameChunk.getNeighbors());
			if (newMainChrCoordinates == npCharacter->getCoordinatesInGameChunks())
			{
				if (npCharacter->getStunCooldownTimer() == 0)
				{
					return LOSE_STATE;
				}
			}

			mainCharacter->move(nextState, newMainChrCoordinates);
			break;

		case INTERACT:
			if (!mainCharacter->canTeleport() || mainCharacter->hasTeleportCooldown())
			{
				break;
			}

			if (crtGameChunk.getNumberOfNeighbors() == 1)
			{
				newMainChrCoordinates = portals[rand() % (numberOfPortals - 1)];
				if (mainCharacter->getCoordinatesInGameChunks() == npCharacter->getCoordinatesInGameChunks())
				{
					if (npCharacter->getStunCooldownTimer() == 0)
					{
						return LOSE_STATE;
					}
				}
				mainCharacter->move(MOVE_UP, newMainChrCoordinates);
				mainCharacter->triggerTeleportCooldown();
			}
			break;

		case ATTACK:
			if (!mainCharacter->hasAttackCooldown())
			{
				std::pair<int, int> attackCoordinates = mainCharacter->attack();
				std::pair<int, int> npcCoordinates = npCharacter->getCoordinatesInGameChunks();
				if (attackCoordinates.first != -1)
				{
					if (abs(npcCoordinates.first - attackCoordinates.first) <= 1 && abs(npcCoordinates.second - attackCoordinates.second) <= 1)
					{
						npCharacter->triggerStunCooldownTimer();
					}
				}
			}
			break;

		default:
			break;
		}

		if (!key->isConsumed() && !key->isHidden() && mainCharacter->getCoordinatesInGameChunks() == key->getCoordinates())
		{
			mainCharacter->getKey();
			key->consume();
		}

		if (!teleportAbility->isConsumed() && !teleportAbility->isHidden() && mainCharacter->getCoordinatesInGameChunks() == teleportAbility->getCoordinates())
		{
			mainCharacter->unlockTeleport();
			teleportAbility->consume();
		}

		return GAME_STATE;
	}

	MainState process_game_state(InputState inputState, PlayerCharacter* mainCharacter, GameProp* exitKey, GameProp* teleportAbility, NonplayerCharacter* npCharacter, GameChunk* gameChunks, std::pair<int, int>* portals, int numberOfPortals)
	{
		switch (inputState)
		{
		case PAUSE:
			return PAUSE_STATE;
			break;
		default:
			return process_player_state(mainCharacter, exitKey, teleportAbility, npCharacter, inputState, gameChunks[mainCharacter->getCoordinatesInGameChunks().second * WIDTH + mainCharacter->getCoordinatesInGameChunks().first], portals, numberOfPortals);
		}
	}

	CharacterState get_npc_direction(std::pair<int, int> prevCoordinates, std::pair<int, int> newCoordinates)
	{
		CharacterState newCharacterState = prevCoordinates.first - newCoordinates.first > 0 ? MOVE_LEFT :
			prevCoordinates.first - newCoordinates.first < 0 ? MOVE_RIGHT :
			prevCoordinates.second - newCoordinates.second > 0 ? MOVE_UP :
			prevCoordinates.second - newCoordinates.second < 0 ? MOVE_DOWN : IDLE;

		return newCharacterState;
	}

	paco::DL_List* process_npc_state(NonplayerCharacter* npCharacter, paco::Node* goal, GameChunk crtGameChunk, paco::DL_List* existingPath)
	{
		existingPath = paco::get_next_move(crtGameChunk.getPathNode(), goal, existingPath);
		if (!existingPath || !existingPath->first)
		{
			std::cerr << "Path is empty!\n";
			return NULL;
		}
		std::pair<int, int> nextCoordinates = existingPath->first->coordinates;
		paco::delete_first(existingPath);
		CharacterState state = get_npc_direction(npCharacter->getCoordinatesInGameChunks(), nextCoordinates);
		npCharacter->move(state, nextCoordinates);

		return existingPath;
	}
}