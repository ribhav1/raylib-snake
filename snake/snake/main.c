#include "string.h"
#include "raylib.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
char title[64] = "My game";

const int cells = 20;
int cell_x = 0;
int cell_y = 0;

char dir[6] = "RIGHT";

int main(int argc, int arv[])
{
    int cell_width = WINDOW_WIDTH / cells;
    int cell_height = WINDOW_HEIGHT / cells;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title);
    SetTargetFPS(16);

    float elapsed = 0.0f;
    float move_threshold = 0.125f;

    bool game_over = false;
    while (!WindowShouldClose())
    {
        // update physics
        int xpos = cell_x * cell_width;
        int ypos = cell_y * cell_height;

        // only move snake every move_threshold seconds
        elapsed += GetFrameTime();
        if (elapsed >= move_threshold && !game_over)
        {
            if (strcmp(dir, "LEFT") == 0)
            {
                if (cell_x == 0) game_over = true;
                cell_x -= 1;
            }
            else if (strcmp(dir, "RIGHT") == 0)
            {
                if (cell_x == cells - 1) game_over = true;
                cell_x += 1;
            }
            else if (strcmp(dir, "UP") == 0)
            {
                if (cell_y == 0) game_over = true;
                cell_y -= 1;
            }
            else if (strcmp(dir, "DOWN") == 0)
            {
                if (cell_y == cells - 1) game_over = true;
                cell_y += 1;
            }

            elapsed -= move_threshold;
        }

        if (IsKeyPressed(KEY_W))
        {
            strcpy_s(dir, 6, "UP");
        }
        if (IsKeyPressed(KEY_A))
        {
            strcpy_s(dir, 6, "LEFT");
        }
        if (IsKeyDown(KEY_S))
        {
            strcpy_s(dir, 6, "DOWN");
        }
        if (IsKeyPressed(KEY_D))
        {
            strcpy_s(dir, 6, "RIGHT");
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
                cell_x = 0;
                cell_y = 0;
                strcpy_s(dir, 6, "RIGHT");
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