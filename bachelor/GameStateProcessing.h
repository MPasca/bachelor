#pragma once

#include "StateProcessing.h"

#include "Character.h"
#include "GameChunk.h"
#include "GameProp.h"

namespace stateprocessing
{
	MainState process_game_state(inputdcd::InputState inputState, PlayerCharacter* mainCharacter, GameProp* exitKey, GameProp* teleportAbility, NonplayerCharacter* npCharacter, GameChunk* gameChunks, std::pair<int, int>* portals, int numberOfPortals);

	paco::DL_List* process_npc_state(NonplayerCharacter* npCharacter, paco::Node* goal, GameChunk crtGameChunk, paco::DL_List* existingPath);
}

