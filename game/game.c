#include <game.h>
#include <../snake/snake.h>
#include <../apple/apple.h>

void print_debug() {
    move(0,0);
    printw( "snake x: %d, snake y: %d, maxX: %d, maxY: %d, quit: %d, %d",
            get_head()->m_x,get_head()->m_y,getmaxx(stdscr),getmaxy(stdscr), quit, apples_length());
    apple *a = get_apples();
    while(a != NULL) {
        printw("(x %d, y %d) ", a->m_x, a->m_y);
        a = a->ptr_next;
    }
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
        head->m_x < 0 || head_y < 1) {
            quit = true;
        }
    //check if snake overlaps
    snake *s = get_head()->ptr_next;
    while(s != NULL) {
        if(s->m_x == get_head()->m_x && s->m_y == get_head()->m_y) {
            quit = true;
            break;
        }
        s = s->ptr_next;
    }
    //check if apple is hit
    apple *a = get_apples();
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
    spawn_apples();
    while (!quit)
    {
        clear();
        check_for_collision();
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
    snake *s = head, *t = NULL;
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

snake *get_head() {
    return head;
}

void run() 
{
    setup();
    head = setup_snake(START_TAIL_LENGHT+1,10);
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
