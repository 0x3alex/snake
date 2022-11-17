#include <apple.h>
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
    snake *s = get_head();
    while(s != NULL) {
        if(s->m_x == a->m_x && s->m_y == a->m_y) {
            return true;
        }
        s = s->ptr_next;
    }
    return false;
}

bool apple_exists(apple *ap) {
    apple *a = get_apples();
    while(a != NULL) {
        if(a != ap && a->m_x == ap->m_x && a->m_y == ap->m_y) {
            return true;
        }
        a = a->ptr_next;
    }
    return false;
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
    a->ptr_next->m_x = select_random_x();
    a->ptr_next->m_y = select_random_y();
    while(  apple_overlap_snake(a) ||
            apple_exists(a)) {
                apples->ptr_next->m_x = select_random_x();
                apples->ptr_next->m_y = select_random_y();
            }
}

void re_gen_apple_x_y(int x, int y) {
    apple *a = apples;
    while(a != NULL) {
        if(a->m_x == x && a->m_y == y) {
            a->m_x = select_random_x();
            a->m_y = select_random_y();
            break;
        }
        a = a->ptr_next;
    }
}


void spawn_apples() {
    apples = (apple*) calloc(1,sizeof(apple));
    assert(apples != NULL);
    for(int i = 0; i < APPLE_COUNT; i++) {
        if(apples == NULL) {
            apples->ptr_next = NULL;
            apples->ptr_prev = NULL;
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
        move(a->m_y,a->m_x);
        addch('*');
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