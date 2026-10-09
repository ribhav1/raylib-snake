#pragma once

#include <raylib.h>

// linked list data structure to keep track of snake
typedef struct body_node_t
{
	int x;
	int y;
	struct body_node_t* next;
} body_t;

typedef struct
{
	int length;

	body_t* tail;
	body_t* head;

	Vector2 dir;
	Vector2 last_dir; // to avoid bugs with multiple direction changes between movement updates
} snake_t;

int snake_init(snake_t *snake, int start_x, int start_y);

void snake_destroy(snake_t *snake);