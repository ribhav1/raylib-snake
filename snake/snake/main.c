#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "raylib.h"

// window settings
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
char title[64] = "My game";

// define the width/height of grid
#define CELLS 15

// keep track of direction o fhead of snake
Vector2 dir;

// linked list data structure to keep track of snake
typedef struct body_node_t
{
    int x;
    int y;
    struct body_node_t* next;
} body_t;

// map of grid for collisision checks
bool grid[CELLS][CELLS];

// forward declaration for initializing snake
body_t* initialize_snake(body_t *tail, bool grid[CELLS][CELLS]);

int main(int argc, int argv[])
{
    SetTargetFPS(60);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title);

    int cell_width = WINDOW_WIDTH / CELLS;
    int cell_height = WINDOW_HEIGHT / CELLS;

    // track time elapsed since last direction update
    // and set threshold for how often direction should be updated
    float elapsed = 0.0f;
    float move_threshold = 0.1f;

    // set initial move direction towards right side
    dir.x = 1;
    dir.y = 0;

    // initialize snake and grid
    body_t *tail = malloc(sizeof(body_t));
    tail->x = 0;
    tail->y = 0;
    body_t* head = initialize_snake(tail, grid);

    // generate initial apple spawn pos
    srand(time(NULL));
    int apple_pos[] = { rand() % CELLS, rand() % CELLS };

    // game running or not flag
    bool game_over = false;
    while (!WindowShouldClose())
    {
        // check if game ended then offer option to reset if so
        if (game_over)
        {
            DrawText("YOU LOST!", 50, 200, 130, RED);
            DrawText("Press Y to reset", 200, 400, 50, GREEN);
            DrawText("Press N to exit", 200, 450, 50, GREEN);

            if (IsKeyPressed(KEY_Y))
            {
                game_over = false;
                elapsed = 0.0f;

                // reset snake list to empty
                body_t* curr = tail;
                while (curr != NULL)
                {
                    body_t* to_free = curr;
                    curr = curr->next;

                    free(to_free);
                }

                // reinitialize snake and grid
                tail = malloc(sizeof(body_t));
                tail->x = 0;
                tail->y = 0;
                head = initialize_snake(tail, grid);

                // reset direction
                dir.x = 1;
                dir.y = 0;  
            }
            else if (IsKeyPressed(KEY_N))
            {
                break;
            }
        }
        // otherwise poll for inputs
        else
        { 
            // make sure snake can't do 180 degree turns as well
            if (IsKeyPressed(KEY_W) && dir.y != 1)
            {
                dir.x = 0;
                dir.y = -1;
            }
            else if (IsKeyPressed(KEY_A) && dir.x != 1)
            {
                dir.x = -1;
                dir.y = 0;
            }
            else if (IsKeyPressed(KEY_S) && dir.y != -1)
            {
                dir.x = 0;
                dir.y = 1;
            }
            else if (IsKeyPressed(KEY_D) && dir.x != -1)
            {
                dir.x = 1;
                dir.y = 0;
            }
        }

        // process movement
        // only move snake every move_threshold seconds
        elapsed += GetFrameTime();

        // process every move interval that elapsed since the last frame.
        // since multiple intervals can elapse during a single frame if a large frametime occurs
        // use a while loop to catch up without moving early on a subsequent frame.
        while (elapsed >= move_threshold)
        {
            if (!game_over)
            {
                // calculate next position of head
                int new_x = head->x + dir.x;
                int new_y = head->y + dir.y;

                // check boundaries and collisions
                if (new_x < 0 || new_x >= CELLS || new_y < 0 || new_y >= CELLS)
                {
                    game_over = true;
                    break;
                }

                // if the head has moved to an already occupied space/collided with itself then game is over
                if (grid[new_y][new_x])
                {
                    game_over = true;
                    break;
                }

                // move snake by moving adding body node in current direction and removing tail
                body_t* new_head = malloc(sizeof(body_t)); // create new head

                // initialize new head
                new_head->x = new_x;
                new_head->y = new_y;
                new_head->next = NULL;

                // append new head to end of list
                head->next = new_head;
                head = new_head;

                // mark new position as in use
                grid[new_y][new_x] = true;

                // if not on apple, remove tail
                if (head->x != apple_pos[0] || head->y != apple_pos[1])
                {
                    body_t* old_tail = tail;
                    tail = tail->next; // move tail forward

                    grid[old_tail->y][old_tail->x] = false; // set old tail position as not in use anymore
                    free(old_tail);
                }

                // generate new apple position if snake did eat it
                else
                {
                    int new_apple_x = rand() % CELLS;
                    int new_apple_y = rand() % CELLS;
                    while (grid[new_apple_y][new_apple_x])
                    {
                        new_apple_x = rand() % CELLS;
                        new_apple_y = rand() % CELLS;
                    }
                    apple_pos[0] = new_apple_x;
                    apple_pos[1] = new_apple_y;
                }
            }

            elapsed -= move_threshold;
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

        // draw snake
        body_t* curr = tail;
        while (curr != NULL)
        {
            DrawRectangle(curr->x * cell_width, curr->y * cell_height, cell_width, cell_height, GREEN);
            
            // draw face on head of snake
            if (curr == head)
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
        DrawRectangle(apple_pos[0] * cell_width, apple_pos[1] * cell_height, cell_width, cell_height, RED);

        EndDrawing();
    }

    // free snake
    body_t* curr = tail;
    while (curr != NULL)
    {
        body_t* to_free = curr;
        curr = curr->next;

        free(to_free);
    }

    CloseWindow();
    return 0;
}

body_t* initialize_snake(body_t *tail, bool grid[CELLS][CELLS])
{
    // reset snake
    tail->next = malloc(sizeof(body_t));

    body_t* second = tail->next;
    second->x = tail->x + 1;
    second->y = tail->y;
    second->next = malloc(sizeof(body_t));

    body_t* head = second->next;
    head->x = second->x + 1;
    head->y = second->y;
    head->next = NULL;

    // reset grid
    memset(grid, 0, CELLS * CELLS * sizeof(bool));
    body_t* curr = tail;
    while (curr != NULL)
    {
        grid[curr->y][curr->x] = true;
        curr = curr->next;
    }

    return head;
}