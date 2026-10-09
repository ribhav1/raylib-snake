#include "snake.h"
#include <stdlib.h>

// create a snake with 3 body members in a horizontal line starting from start_x
int snake_init(snake_t *snake, int start_x, int start_y)
{
    if (snake == NULL ||
        start_x < 0 || 
        start_y < 0)
    {
        return 0;
    }

    // initialize length
    snake->length = 3;

    // set dir
    snake->dir.x = 1;
    snake->dir.y = 0;

    snake->last_dir = snake->dir;

    // create its body
    snake->tail = malloc(sizeof(body_t)); // tail
    body_t* tail = snake->tail;
    tail->x = start_x;
    tail->y = start_y;

    tail->next = malloc(sizeof(body_t)); // middle
    body_t* second = tail->next;
    second->x = start_x + 1;
    second->y = start_y;

    second->next = malloc(sizeof(body_t)); // head
    body_t* head = second->next;
    head->x = start_x + 2;
    head->y = start_y;

    head->next = NULL;
    snake->head = head;

	return 1;
}

void snake_destroy(snake_t *snake)
{
	// free all linked list members
    body_t* curr = snake->tail;
    while (curr != NULL)
    {
        body_t* to_free = curr;
        curr = curr->next;

        free(to_free);
    }

    free(snake);
}