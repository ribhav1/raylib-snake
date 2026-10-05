#include "string.h"
#include "raylib.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
char title[64] = "My game";

const int cells = 12;
int cell_x = 0;
int cell_y = 0;

Vector2 dir;

int main(int argc, int arv[])
{
    SetTargetFPS(60);

    int cell_width = WINDOW_WIDTH / cells;
    int cell_height = WINDOW_HEIGHT / cells;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title);

    // track time elapsed since last direction update
    // and set threshold for how often direction should be updated
    float elapsed = 0.0f;
    float move_threshold = 0.1f;

    // set initial move direction towards right side
    dir.x = 1;
    dir.y = 0;

    bool game_over = false;
    while (!WindowShouldClose())
    {
        // update physics
        int xpos = cell_x * cell_width;
        int ypos = cell_y * cell_height;

        // only move snake every move_threshold seconds
        elapsed += GetFrameTime();

        // process every move interval that elapsed since the last frame.
        // since multiple intervals can elapse during a single frame if a large frametime occurs
        // use a while loop to catch up without moving early on a subsequent frame.
        while (elapsed >= move_threshold)
        {
            if (!game_over)
            {
                cell_x += dir.x;
                cell_y += dir.y;

                if (cell_x < 0 || cell_x >= cells || cell_y < 0 || cell_y >= cells)
                {
                    game_over = true;
                }
            }
            
            elapsed -= move_threshold;
        }

        if (IsKeyPressed(KEY_W))
        {
            dir.x = 0;
            dir.y = -1;
        }
        if (IsKeyPressed(KEY_A))
        {
            dir.x = -1;
            dir.y = 0;
        }
        if (IsKeyPressed(KEY_S))
        {
            dir.x = 0;
            dir.y = 1;
        }
        if (IsKeyPressed(KEY_D))
        {
            dir.x = 1;
            dir.y = 0;
        }

        // update render every frame
        BeginDrawing();
        ClearBackground(RAYWHITE);


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

        DrawRectangle(xpos, ypos, cell_width, cell_height, GREEN);

        if (game_over)
        {
            DrawText("YOU LOST!", 50, 200, 130, RED);
            DrawText("Press Y to reset", 200, 400, 50, GREEN);
            DrawText("Press N to exit", 200, 450, 50, GREEN);

            if (IsKeyPressed(KEY_Y))
            {
                game_over = false;
                
                elapsed = 0.0f;

                cell_x = 0;
                cell_y = 0;

                dir.x = 1;
                dir.y = 0;
            }
            else if (IsKeyPressed(KEY_N))
            {
                CloseWindow();
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}