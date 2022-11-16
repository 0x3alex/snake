#include <game.h>
#include <../snake/snake.h>
#include <../apple/apple.h>

void printDebug() {
    move(0,0);
    printw( "snake x: %d, snake y: %d, maxX: %d, maxY: %d, quit: %d, %d",
            head->m_x,head->m_y,getmaxx(stdscr),getmaxy(stdscr), quit, apples_length());
    apple *a = apples;
    while(a != NULL) {
        printw("(x %d, y %d) ", a->m_x, a->m_y);
        a = a->ptr_next;
    }
}

void check_for_collision() 
{   
    int head_x = head->m_x, head_y = head->m_y;
    if( head->m_x >= getmaxx(stdscr) || 
        head_y >= getmaxy(stdscr) ||
        head->m_x < 0 || head_y < 1) {
            quit = true;
        }
    //check if snake overlaps
    snake *s = head->ptr_next;
    while(s != NULL) {
        if(s->m_x == head->m_x && s->m_y == head->m_y) {
            quit = true;
            break;
        }
        s = s->ptr_next;
    }
    //check if apple is hit
    apple *a = apples;
    while(a != NULL) {
        if(a->m_x == head_x && a->m_y == head_y) {
            re_gen_apple_x_y(a->m_x,a->m_y);
            
            push_to_snake();
            
            break;
        }
        a = a->ptr_next;
    }
}

void *draw_thread_func() 
{
    while (!quit)
    {
        clear();
        draw_apples();
        draw_snake();
        move_snake();
        #ifdef DEV_MODE
        printDebug();
        #endif
        check_for_collision();
        usleep(100000);
        refresh();
    }
    clear();
    printw("You lost, your snake was %d blocks long!\nPress any key to quit!", snake_length() - START_TAIL_LENGHT);
    refresh();
    return NULL;
    
}

void setup() 
{
    WINDOW *w = initscr();
    assert(w != NULL);
    raw();
    curs_set(0);
    noecho();
    start_color();
    keypad(stdscr,TRUE);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srandom((time_t)ts.tv_nsec);
    init_pair(1,COLOR_RED,COLOR_RED);
    init_pair(2,COLOR_GREEN,COLOR_GREEN);
    clear();
}


void freeAll() 
{
    //free snake
    snake *s = get_last(), *t = NULL;
    while (s != NULL)
    {
        t = s->ptr_prev;
        free(s);
        s = t;
    }

    //free apples
    apple *a = get_last_apple(), *b = NULL;
    while (a != NULL)
    {
        b = a->ptr_prev;
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
    head = setup_snake(10,10);
    pthread_create(&draw_thread,NULL,draw_thread_func,(void*)draw_thread);
    pthread_detach(draw_thread);
    pthread_create(&apple_thread,NULL,apple_thread_func,(void*)apple_thread);
    pthread_detach(apple_thread);
    int key = 0;
    while(!quit) {
        key = getch();
        if(quit) break;
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
