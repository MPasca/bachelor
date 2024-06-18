#pragma once
#include<iostream>
#undef main

#include "input_processing/InputProcessing.h"
#include "GameStateProcessing.h"
#include "MenusStateProcessing.h"

#include "Utils.h"

InputState getNextSubstate(SDL_Keycode key);

CharacterState convert_input_to_player_state(InputState inputState);

MainState process_menu_state(InputState inputState, Menu* crtMenu);

MainState process_game_state(InputState inputState, PlayerCharacter* mainCharacter, NonplayerCharacter* npc, GameChunk* gameChunks, std::pair<int, int>* portals, int numberOfPortals);

void process_npc_state(NonplayerCharacter* npCharacter, paco::Node* goal, GameChunk crtGameChunk);