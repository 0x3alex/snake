#include "apple.h"
#include <stdlib.h>
#include <ncurses.h>
#include <assert.h>
#include "../snake/snake.h"
#include "../game/game.h"

apple *get_apples() {
    return apples;
}

int select_random_x() {
    return (rand() %
        (getmaxx(stdscr) - 5 + 1)) + 5;
}

int select_random_y() {
    return (rand() %
        (getmaxy(stdscr) - 5 + 1)) + 5;
}

bool apple_overlap_snake(apple *a) {
    return mvinch(a->m_y,a->m_x) == SNAKE_CHAR;
}

bool apple_exists(apple *a) {
    return  mvinch(a->m_y,a->m_x) == '*' ||
            mvinch(a->m_y+1,a->m_x) == '*' ||
            mvinch(a->m_y-1,a->m_x) == '*' ||
            mvinch(a->m_y,a->m_x+1) == '*' ||
            mvinch(a->m_y,a->m_x-1) == '*';
}

apple *get_last_apple() {
    apple *a = apples;
    while(a->ptr_next != NULL) a = a->ptr_next;
    return a;
}

void push_apple() 
{
    apple *a = get_last_apple();
    a->ptr_next = (apple*) calloc(1,sizeof(apple));
    assert(a->ptr_next != NULL);
    a->ptr_next->ptr_next = NULL;
    a->ptr_next->ptr_prev = a;
    a->ptr_next->ate = false;
    a->ptr_next->m_x = select_random_x();
    a->ptr_next->m_y = select_random_y();
    while(  apple_overlap_snake(a) ||
            apple_exists(a)) {
                apples->ptr_next->m_x = select_random_x();
                apples->ptr_next->m_y = select_random_y();
            }
}

void re_gen_apple_x_y(apple *a) {
    a->m_x = select_random_x();
    a->m_y = select_random_y();

    while(  apple_overlap_snake(a) ||
            apple_exists(a)) {
                a->m_x = select_random_x();
                a->m_y = select_random_y();
            }
    a->ate = false;
}


void spawn_apples() {
    apples = (apple*) calloc(1,sizeof(apple));
    assert(apples != NULL);
    for(int i = 0; i < APPLE_COUNT; i++) {
        if(apples == NULL) {
            apples->ptr_next = NULL;
            apples->ptr_prev = NULL;
            apples->ate = false;
            apples->ptr_next->m_x = select_random_x();
            apples->ptr_next->m_y = select_random_y();
            while(  apple_overlap_snake(apples) || apple_exists(apples)) {
                        apples->ptr_next->m_x = select_random_x();
                        apples->ptr_next->m_y = select_random_y();
                    }
            continue;
        }
        push_apple();
    }
}

void draw_apples() {
    apple *a = apples;
    attron(COLOR_PAIR(1));
    while(a != NULL) {
        if(a->m_x == 0 && a->m_y == 0) {
            a = a->ptr_next;
            continue;
        }
        if(!a->ate) {
            move(a->m_y,a->m_x);
            addch('*');
        }
        
        a = a->ptr_next;
    }
    attroff(COLOR_PAIR(2));
}

int apples_length() {
    int i = 0; 
    apple *a = apples;
    while(a != NULL) {
        i++;
        a = a->ptr_next;
    }
    return i;
}