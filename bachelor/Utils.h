#pragma once

// character attributes
#define INITIAL_LIVES 3
#define INITIAL_HEALTH_POINTS 5
#define COOLDOWN 7

// gamesurface attributes
#define HEIGHT 9
#define WIDTH 15
#define GAME_CHUNK 64
#define BUTTON_WIDTH 320
#define BUTTON_HEIGHT 128

// assets paths
#define ASSETS_PATH "./assets"
#define MAZE_PATH "./assets/maze/"
#define PLAYERCHR_PATH "./assets/player-chr"
#define PROP_PATH "./assets/prop"
#define NPC_PATH "./assets/npc"


typedef enum {
	IDLE, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, INTERACT, ATTACK, HURT
} CharacterState;

typedef enum {
	TITLE_SCREEN, GAME, PAUSE_MENU, WIN_SCREEN, LOSE_SCREEN, EXIT, NO_CHANGE
}MainState;

typedef enum {
	IDLE_INPUT, ARROW_DOWN, ARROW_UP, ARROW_LEFT, ARROW_RIGHT, INTERACT_INPUT, ATTACK_INPUT, PAUSE
}InputState;