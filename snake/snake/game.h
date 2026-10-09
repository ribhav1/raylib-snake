#pragma once

#include <raylib.h>
#include "snake.h"

// window settings
typedef struct
{
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	char title[64];
} window_options;

typedef struct
{
	bool game_over;
	float elapsed;
	float move_threshold;

	int cells;

	int cell_width;
	int cell_height;

	int** collision_grid;

	snake_t *snake;
	Vector2 apple_pos;
} game_t;


int game_initialize_window_options(window_options* options, int WINDOW_WIDTH, int WINDOW_HEIGHT, char* title);

void game_generate_new_apple_pos(game_t* game);

int game_init(game_t* game, window_options *options, int cells, float move_threshold);

bool game_poll_input(game_t *game);

void game_update(game_t *game, float delta_time);

void game_reset_state(game_t* game);


void game_destroy(game_t* game);