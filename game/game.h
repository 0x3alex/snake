#pragma once

#include <ncurses.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define KEY_QUIT 'q'

#ifdef DEV_MODE
#define DEV_KEY 'g'
#endif

static pthread_t draw_thread, apple_thread;
static bool quit = false, over = false;

#ifdef DEV_MODE
void printDebug();
#endif

void check_for_collision();
void *draw_thread_func();
void setup();
void freeAll();
void run();