#pragma once

#include <ncurses.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <../snake/snake.h>

#define KEY_QUIT 'q'

#ifdef DEV_MODE
#define DEV_KEY 'g'
#endif

static pthread_t draw_thread;
static bool quit = false, over = false;

static snake *head;

snake *get_head();

#ifdef DEV_MODE
void print_debug();
#endif

void check_for_collision();
void *draw_thread_func();
void setup();
void freeAll();
void run();