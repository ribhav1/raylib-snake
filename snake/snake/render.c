#include "game.h"
#include "render.h"

void render_draw(game_t *game, window_options *options)
{
    if (game->game_over)
    {
        ClearBackground(WHITE);

        DrawText("GAME OVER!", 50, 200, 145, RED);
        DrawText("Press Y to reset", 200, 400, 70, GREEN);
        DrawText("Press N to exit", 200, 450, 70, GREEN);

        return;
    }

    int cell_width = game->cell_width;
    int cell_height = game->cell_height;

    int WINDOW_WIDTH = options->WINDOW_WIDTH;
    int WINDOW_HEIGHT = options->WINDOW_HEIGHT;

    snake_t* snake = game->snake;

    // draw vertical grid lines
    for (int xpos = cell_width; xpos <= WINDOW_WIDTH - cell_width; xpos += cell_width)
    {
        DrawLine(xpos, 0, xpos, WINDOW_HEIGHT, BLACK);
    }

    // draw horizontal grid lines
    for (int ypos = cell_height; ypos <= WINDOW_HEIGHT - cell_height; ypos += cell_height)
    {
        DrawLine(0, ypos, WINDOW_WIDTH, ypos, BLACK);
    }

    // draw snake
    body_t* curr = snake->tail;
    while (curr != NULL)
    {
        DrawRectangle(curr->x * cell_width, curr->y * cell_height, cell_width, cell_height, GREEN);

        // draw face on head of snake
        if (curr == snake->head)
        {
            int x = curr->x * cell_width;
            int y = curr->y * cell_height;

            // eyes
            DrawRectangle(x + cell_width / 4,
                y + cell_height / 4,
                cell_width / 7,
                cell_height / 7,
                BLACK);

            DrawRectangle(x + 5 * cell_width / 8,
                y + cell_height / 4,
                cell_width / 7,
                cell_height / 7,
                BLACK);

            // mouth
            DrawRectangle(x + cell_width / 3,
                y + 3 * cell_height / 5,
                cell_width / 3,
                cell_height / 10,
                BLACK);
        }
        curr = curr->next;
    }

    // draw apple
    DrawRectangle(game->apple_pos.x * cell_width, game->apple_pos.y * cell_height, cell_width, cell_height, RED);
}