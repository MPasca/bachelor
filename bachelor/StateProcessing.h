#pragma once
#include<iostream>
#undef main

#include "input_processing/InputProcessing.h"
#include "GameStateProcessing.h"
#include "Utils.h"

#include "Character.h"
#include "GameChunk.h"
#include "Menu.h"
#include "Button.h"

MainState process_menu_state(InputState inputState, Menu* crtMenu);

MainState process_game_state(InputState inputState, PlayerCharacter* mainCharacter, NonplayerCharacter* npc, GameChunk* gameChunks, std::pair<int, int>* portals, int numberOfPortals);