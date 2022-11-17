#pragma once

#define START_TAIL_LENGHT 30
#define SNAKE_CHAR 'O'

#define X_UP 'w'
#define X_DOWN 's'
#define Y_UP 'd'
#define Y_DOWN 'a'


enum oritentation {PX,PY,NX,NY};
typedef struct s_snake
{
    int m_x, m_y;
    enum oritentation m_orientation;
    struct s_snake *ptr_next,*ptr_prev;
}snake;



snake *setup_snake(int x, int y);
snake *get_last();
void change_orientation(int key);
void push_to_snake();
void draw_snake();
void move_snake();
const int snake_length();