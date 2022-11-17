#include <snake.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>
#include <../game/game.h>

snake *head;

snake *get_head() {
    return head;
}

void setup_snake(int x, int y) 
{
    //Setup get_head()
    head = (snake*) calloc(1,sizeof(snake));
    snake *t = NULL;
    assert(head != NULL);
    head->m_x = x;
    head->m_y = y;
    head->ptr_next = NULL;
    head->ptr_prev = NULL;
    head->m_orientation = PX;
    t = head;
    //setup tail
    for(unsigned int i = 0; i < START_TAIL_LENGHT; i++) {
        t->ptr_next = (snake*) calloc(1,sizeof(snake));
        assert(t->ptr_next != NULL);
        t->ptr_next->ptr_prev = t;
        t->ptr_next->m_x = t->m_x-1;
        t->ptr_next->m_y = t->m_y;
        t->ptr_next->ptr_next = NULL;
        t = t->ptr_next;
    }
}

snake *get_last() 
{
    snake *t = get_head();
    while(t->ptr_next != NULL) t = t->ptr_next;
    return t;
}

void change_orientation(int key) 
{
    switch (key)
    {
        case X_UP:
            if( get_head()->m_orientation == PX || 
                get_head()->m_orientation == NX) {
                    get_head()->m_orientation = PY;
                }
            break;
        case X_DOWN:
            if( get_head()->m_orientation == PX || 
                get_head()->m_orientation == NX) {
                    get_head()->m_orientation = NY;
                }
            break;
        case Y_UP:
            if( get_head()->m_orientation == PY || 
                get_head()->m_orientation == NY) {
                    get_head()->m_orientation = PX;
                }
            break;
        case Y_DOWN:
            if( get_head()->m_orientation == PY || 
                get_head()->m_orientation == NY) {
                    get_head()->m_orientation = NX;
                }
            break;
    }
}

void push_to_snake() 
{
    snake *s = get_head();
    while(s->ptr_next != NULL) s = s->ptr_next;
    s->ptr_next =(snake*) calloc(1,sizeof(snake));
    assert(s->ptr_next != NULL);
    s->ptr_next->m_x = s->m_x;
    s->ptr_next->m_y = s->m_y;
    s->ptr_next->ptr_prev = s;
}

const int snake_length()
{
    snake *s = get_head();
    int i = 0;
    while(s != NULL) {
        i++;
        s = s->ptr_next;
    }
    return i;
}

void draw_snake() 
{
    snake *s = get_head();
    attron(COLOR_PAIR(2));
    while(s != NULL) {
        move(s->m_y,s->m_x);
        addch(SNAKE_CHAR);
        s = s->ptr_next;
    }
    move(get_head()->m_y,get_head()->m_x);
    attroff(COLOR_PAIR(2));
}

void move_snake() {
    int old_x = get_head()->m_x, 
        old_y = get_head()->m_y,
        t_x = 0,
        t_y = 0;
    snake *s = get_head()->ptr_next;
    switch(get_head()->m_orientation) {
        case PX:
            get_head()->m_x++;
            break;
        case NX:
            get_head()->m_x--;
            break;
        case PY:
            get_head()->m_y--;
            break;
        case NY:
            get_head()->m_y++;
            break;
    }
    while(s != NULL) {
        t_x = s->m_x;
        t_y = s->m_y;
        s->m_x = old_x;
        s->m_y = old_y;
        old_x = t_x;
        old_y = t_y;
        s = s->ptr_next;
    }
}