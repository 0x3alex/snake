#include "game.h"

int char_at_head = ' ';

void print_debug() {
    move(0,0);
    printw( "snake x: %d, snake y: %d, maxX: %d, maxY: %d, quit: %d, %d",
            get_head()->m_x,get_head()->m_y,getmaxx(stdscr),getmaxy(stdscr), quit, apples_length());
    printw(" -- %c, %c",char_at_head, SNAKE_CHAR);
}

void print_apple_count() {
    move(0,0);
    printw("Apples ate: %d", snake_length() - START_TAIL_LENGHT -1);
}

void check_for_collision() 
{   
    int head_x = get_head()->m_x, head_y = get_head()->m_y;
    if( head_x >= getmaxx(stdscr) || 
        head_y >= getmaxy(stdscr) ||
        head_x < 0 || head_y < 1) {
            quit = true;
        }
    char_at_head = mvinch(head_y,head_x);
    //check if snake overlaps
    if((char)char_at_head == SNAKE_CHAR) quit = true;
    //check if apple is hit
    if((char)char_at_head == '*') {
        apple *a = find_apple_by_x_y(head_x,head_y);
        if (a != NULL) {
            find_apple_by_x_y(head_x,head_y)->ate = true;
            re_gen_apple_x_y(find_apple_by_x_y(head_x,head_y));
            push_to_snake();
        }
    }
}

void *draw_thread_func() 
{
    spawn_apples();
    while (!quit)
    {
        check_for_collision();
        clear();
        print_apple_count();
        draw_apples();
        draw_snake();
        move_snake();
        #ifdef DEV_MODE
        print_debug();
        #endif
        usleep(100000);
        refresh();
    }
    clear();
    printw("You lost, you ate %d apples!\nPress any key to quit", snake_length() - START_TAIL_LENGHT -1);
    refresh();
    pthread_exit(0);
    return NULL;
    
}

void setup() 
{
    WINDOW *w = initscr();
    assert(w != NULL);
    setup_snake(START_TAIL_LENGHT+1,10);
    raw();
    curs_set(0);
    noecho();
    start_color();
    keypad(stdscr,TRUE);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);
    init_pair(1,COLOR_RED,0);
    init_pair(2,COLOR_GREEN,COLOR_GREEN);
    clear();
}


void freeAll() 
{
    //free snake
    snake *s = get_head(), *t = NULL;
    while (s != NULL)
    {
        t = s->ptr_next;
        free(s);
        s = t;
    }

    //free apples
    apple *a = get_apples(), *b = NULL;
    while (a != NULL)
    {
        b = a->ptr_next;
        free(a);
        a = b;
    }
    clear();
    curs_set(1);
    endwin();
    
}

void run() 
{
    setup();
    pthread_create(&draw_thread,NULL,draw_thread_func,(void*)draw_thread);
    pthread_detach(draw_thread);
    int key = 0;
    while(!quit) {
        key = getch();
        switch(key) {
            case KEY_QUIT:
                quit = true;
                break;
            case X_DOWN: case X_UP: case Y_DOWN: case Y_UP:
                change_orientation(key);
                break;
            #ifdef DEV_MODE
            case DEV_KEY:  
                push_to_snake();
                break;
            #endif

        }
    }
    
    freeAll();
}
