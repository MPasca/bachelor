#pragma once
#include<iostream>
#undef main

#include "input_processing/InputProcessing.h"
#include "Utils.h"

#include "Character.h"
#include "GameChunk.h"
#include "Menu.h"
#include "Button.h"

void process_player_state(PlayerCharacter* mainCharacter, NonplayerCharacter* npc, InputState inputState, GameChunk crtGameChunk, std::pair<int, int>* portals, int numberOfPortals);

MainState process_menu_state(InputState inputState, Menu* crtMenu);