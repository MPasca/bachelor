#pragma once

// character attributes
#define ATTACK_COOLDOWN 10
#define STUN_COOLDOWN 5
#define TELEPORT_COOLDOWN 20

// gamesurface attributes
#define HEIGHT 9
#define WIDTH 15
#define GAME_CHUNK 64

#define VIEWPORT_WIDTH 1000
#define VIEWPORT_HEIGHT 600

#define BUTTON_WIDTH 128
#define BUTTON_HEIGHT 64

// assets paths
#define MAZE_PATH "./assets/maze/"
#define PLAYERCHR_PATH "./assets/player/"
#define PROP_PATH "./assets/game_props/"
#define NPC_PATH "./assets/npc/"
#define MENUS_SRC_PATH "./assets/menu_obj"

#define ACTIVE_BTN "_active.png"
#define INACTIVE_BTN "_inactive.png"

#include "ErrorCodes.h"

typedef enum {
	IDLE, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, INTERACT, ATTACK
} CharacterState;

typedef enum {
	TITLE_STATE, GAME_STATE, PAUSE_STATE, WIN_STATE, LOSE_STATE, EXIT_STATE, SAME_STATE
}MainState;