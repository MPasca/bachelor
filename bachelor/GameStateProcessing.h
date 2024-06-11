#pragma once
#include<iostream>
#undef main

#include "input_processing/InputProcessing.h"
#include "Utils.h"

#include "Character.h"
#include "GameChunk.h"

MainState process_game_state(InputState inputState, PlayerCharacter* mainCharacter, NonplayerCharacter* npCharacter, GameChunk* gameChunks, std::pair<int, int>* portals, int numberOfPortals);

paco::DL_List* process_npc_state(NonplayerCharacter* npCharacter, paco::Node* goal, GameChunk crtGameChunk, paco::DL_List* existingPath);