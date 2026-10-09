#include "game.h"
#include <string.h>
#include <raylib.h>
#include <time.h>
#include <stdlib.h>

// initialize a window_options struct
int game_initialize_window_options(window_options *options, int WINDOW_WIDTH, int WINDOW_HEIGHT, char *title)
{
	if (options == NULL || title == NULL || 
		WINDOW_WIDTH <= 0 || WINDOW_HEIGHT <= 0 ||
		strlen(title) >= sizeof(options->title))
	{
		return 0;
	}

	options->WINDOW_WIDTH = WINDOW_WIDTH;
	options->WINDOW_HEIGHT = WINDOW_HEIGHT;
	strcpy_s(options->title, 64, title);

	return 1;
}

int game_init(game_t* game, window_options *options, int cells, float move_threshold)
{
    if (game == NULL ||
        options == NULL ||
        cells <= 3 ||
        move_threshold <= 0.0f) 
    {
        return 0;
    }

    // set starting state
	game->game_over = false;

    game->elapsed = 0;
    game->move_threshold = move_threshold;

	game->cells = cells;
	game->cell_width = options->WINDOW_WIDTH / cells;
	game->cell_height = options->WINDOW_HEIGHT / cells;

    // initialize collision grid
	game->collision_grid = malloc(cells * sizeof(int*));
    for (int i = 0; i < cells; i++)
    {
        game->collision_grid[i] = calloc(cells, sizeof(int));
    }

    // initialize snake
    game->snake = malloc(sizeof(snake_t));
    snake_init(game->snake, 0, 0);
        
    // mark snake position as occupied
    game->collision_grid[0][0] = 1;
    game->collision_grid[0][1] = 1;
    game->collision_grid[0][2] = 1;

    srand(time(NULL));
    game_generate_new_apple_pos(game);

    return 1;
}

bool game_poll_input(game_t* game)
{
    // check if game ended then offer option to reset
    if (game->game_over)
    {
        if (IsKeyPressed(KEY_Y))
        {
            // reset game state
            game_reset_state(game);
            return true;
        }
        else if (IsKeyPressed(KEY_N))
        {
            return false;
        }
    }

    // otherwise poll for inputs
    // and make sure snake can't do 180 degree
    Vector2 dir = game->snake->dir;
    Vector2 last_dir = game->snake->last_dir;

    if (IsKeyPressed(KEY_W) && last_dir.y != 1)
    {
        dir.x = 0;
        dir.y = -1;
    }
    else if (IsKeyPressed(KEY_A) && last_dir.x != 1)
    {
        dir.x = -1;
        dir.y = 0;
    }
    else if (IsKeyPressed(KEY_S) && last_dir.y != -1)
    {
        dir.x = 0;
        dir.y = 1;
    }
    else if (IsKeyPressed(KEY_D) && last_dir.x != -1)
    {
        dir.x = 1;
        dir.y = 0;
    }
    
    game->snake->dir = dir;
    return true;
}

void game_update(game_t *game, float delta_time)
{
    // process movement
    // only move snake every move_threshold seconds
    game->elapsed += delta_time;

    // process every move interval that elapsed since the last frame.
    // since multiple intervals can elapse during a single frame if a large frametime occurs
    // use a while loop to catch up without moving early on a subsequent frame.
    while (game->elapsed >= game->move_threshold)
    {
        if (!game->game_over)
        {
            snake_t* snake = game->snake;

            // calculate next position of head
            int new_x = snake->head->x + snake->dir.x;
            int new_y = snake->head->y + snake->dir.y;

            // check boundaries and collisions
            if (new_x < 0 || new_x >= game->cells || new_y < 0 || new_y >= game->cells)
            {
                game->game_over = true;
                break;
            }

            // if the head has moved to an already occupied space/collided with itself then game is over
            if (game->collision_grid[new_y][new_x])
            {
                game->game_over = true;
                break;
            }

            // move snake by moving adding body node in current direction and removing tail
            body_t* new_head = malloc(sizeof(body_t)); // create new head

            // initialize new head
            new_head->x = new_x;
            new_head->y = new_y;
            new_head->next = NULL;

            // append new head to end of list
            snake->head->next = new_head;
            snake->head = new_head;

            // if not on apple, remove tail
            if (snake->head->x != game->apple_pos.x || snake->head->y != game->apple_pos.y)
            {
                body_t* old_tail = snake->tail;
                snake->tail = snake->tail->next; // move tail forward

                game->collision_grid[old_tail->y][old_tail->x] = false; // set old tail position as not in use anymore
                free(old_tail);
            }
            // generate new apple position if snake did eat it
            else
            {
                snake->length++;
                if (snake->length >= game->cells * game->cells)
                {
                    game->game_over = true;
                }

                game_generate_new_apple_pos(game);
            }

            // mark new position as in use
            game->collision_grid[new_y][new_x] = true;

            // update last true movement direction
            snake->last_dir = snake->dir;
        }

        game->elapsed -= game->move_threshold;
    }
}

void game_generate_new_apple_pos(game_t *game)
{
    int new_apple_x = rand() % game->cells;
    int new_apple_y = rand() % game->cells;

    while (game->collision_grid[new_apple_y][new_apple_x])
    {
        new_apple_x = rand() % game->cells;
        new_apple_y = rand() % game->cells;
    }

    game->apple_pos.x = new_apple_x;
    game->apple_pos.y = new_apple_y;
}

void game_reset_state(game_t *game)
{
    game->game_over = false;
    game->elapsed = 0.0f;

    // reset snake
    snake_destroy(game->snake);
    game->snake = malloc(sizeof(snake_t));
    snake_init(game->snake, 0, 0);

    // reset grid
    for (int i = 0; i < game->cells; i++)
    {
        memset(game->collision_grid[i], 0, game->cells * sizeof(int));
    }

    body_t* curr = game->snake->tail;
    while (curr != NULL)
    {
        game->collision_grid[curr->y][curr->x] = true;
        curr = curr->next;
    }

    // reset apple
    game_generate_new_apple_pos(game);
}

void game_destroy(game_t *game) 
{
    for (int i = 0; i < game->cells; i++)
    {
        free(game->collision_grid[i]);
    }
    free(game->collision_grid);

    snake_destroy(game->snake);
}