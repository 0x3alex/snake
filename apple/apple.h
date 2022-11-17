#pragma once
#define APPLE_COUNT 15
#include <stdbool.h>
#include <unistd.h>

typedef struct s_apple 
{
    int m_x,m_y;
    struct s_apple *ptr_next, *ptr_prev;
} apple;

static apple *apples;

apple *get_apples();

int select_random_x();
int select_random_y();
bool apple_exists(apple *ap);
bool apple_overlap_snake(apple *a);
apple *get_last_apple();
void push_apple();
void re_gen_apple_x_y(int x, int y);
void spawn_apples();
void draw_apples();
int apples_length();