#pragma once

// character attributes
#define INITIAL_LIVES 3
#define INITIAL_HEALTH_POINTS 5
#define COOLDOWN 7

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
#define PLAYERCHR_PATH "./assets/player-chr"
#define PROP_PATH "./assets/prop"
#define NPC_PATH "./assets/npc"
#define MENUS_SRC_PATH "./assets/menu_obj"

#define ACTIVE_BTN "_active.png"
#define INACTIVE_BTN "_inactive.png"

typedef enum {
	IDLE, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, INTERACT, ATTACK, HURT
} CharacterState;

typedef enum {
	TITLE_STATE, GAME_STATE, PAUSE_STATE, WIN_STATE, LOSE_STATE, EXIT_STATE, SAME_STATE
}MainState;

typedef enum {
	IDLE_INPUT, ARROW_DOWN, ARROW_UP, ARROW_LEFT, ARROW_RIGHT, INTERACT_INPUT, ATTACK_INPUT, PAUSE
}InputState;