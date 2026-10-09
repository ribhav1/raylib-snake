#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "game.h"
#include "render.h"

// define the width/height of grid
#define CELLS 15

int main(int argc, int argv[])
{
    window_options options;
    if (!game_initialize_window_options(&options, 1000, 1000, "Snake"))
    {
        return 1;
    }

    SetTargetFPS(60);
    InitWindow(options.WINDOW_WIDTH, options.WINDOW_HEIGHT, options.title);

    game_t game;
    if (!game_init(&game, &options, CELLS, 0.1f))
    {
        CloseWindow();
        return 1;
    }

    while (!WindowShouldClose())
    {
        if (!game_poll_input(&game))
        {
            break;
        }

        game_update(&game, GetFrameTime());

        // update render every frame
        BeginDrawing();
        ClearBackground(RAYWHITE);

        render_draw(&game, &options);

        EndDrawing();
    }

    
    game_destroy(&game);
    CloseWindow();
    return 0;
}
