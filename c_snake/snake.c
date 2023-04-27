#include <stdio.h>
#include <stdlib.h>


#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGTH 3
#define NO_DATA 12 // Some random number > 7

uint8_t apple[2] = {NO_DATA, NO_DATA};
uint8_t dir = UP;

struct Snake {
    u_int8_t snake_body[64][2];
    u_int8_t snake_head[2];
    u_int8_t snake_len;
};
typedef struct Snake Snake;

Snake snake;

void reset_snake(void){
    for (int i = 0; i < 64; ++i){
            for(int j = 0; j< 2; ++j){
                snake.snake_body[i][j] = NO_DATA;
            }
        }
        snake.snake_head[0] = rand() % 8; // random(0, 8) in ArduinoIDE
        snake.snake_head[1] = rand() % 8;
        snake.snake_len = 0;
}

void gen_apple(void){
    u_int8_t a_row = rand() % 8;
    u_int8_t a_col = rand() % 8;
    u_int8_t good_apple_pos = 0;
    
    while(!good_apple_pos){
        u_int8_t apple_in_body = 0;
        for(int i = 0; i < snake.snake_len; ++i){
            if(snake.snake_body[i][0] == a_row && snake.snake_body[i][1] == a_col){
                apple_in_body = 1;
                break;
            }
        }
        if ((a_row == snake.snake_head[0] && a_col == snake.snake_head[1]) || apple_in_body){
            a_row = rand() % 8;
            a_col = rand() % 8;
        }
        else{
            good_apple_pos = 1;
        }
    }
    apple[0] = a_row;
    apple[1] = a_col;
}

u_int8_t apple_head_collision(){
    if (apple[0] == snake.snake_head[0] && apple[1] == snake.snake_head[1]){
        return 1;
    }
    else{
        return 0;
    }
}

u_int8_t head_body_collision(){
    for(int i = 0; i < snake.snake_len; ++i){
        if(snake.snake_head[0] == snake.snake_body[i][0] && snake.snake_head[1] == snake.snake_body[i][1]){
            return 1;
        }
    }
    return 0;
}

uint8_t is_snake(int row, int col){
    if(row == snake.snake_head[0] && col == snake.snake_head[1]){
        return 1;
    }
    for(int i = 0; i < snake.snake_len; ++i){
        if(row == snake.snake_body[i][0] && col == snake.snake_body[i][1]){
            return 1;
        }
    }
    return 0;
}

void print_snake(void){
    for(int row = 0; row < 8; ++row){
        for(int col = 0; col < 8; ++col){
            if(row == apple[0] && col == apple[1]){
                printf("o ");
                continue;    
            }
            printf(is_snake(row, col) ? "* ": "- ");
        }
        printf("\n");
    }
    printf("\n");
}


void refresh(void){

    u_int8_t temp_last_row = 0;
    u_int8_t temp_last_col = 0;
    // shift body one back
    if(snake.snake_len > 0){
        temp_last_row = snake.snake_body[snake.snake_len -1][0];
        temp_last_col = snake.snake_body[snake.snake_len -1][0];
    }
    else{
        temp_last_row = snake.snake_head[0];
        temp_last_col = snake.snake_head[1];
    }
    for(int i = snake.snake_len-1; i > 0; --i){
        snake.snake_body[i][0] = snake.snake_body[i -1][0];
        snake.snake_body[i][1] = snake.snake_body[i -1][1];        
    }
    if(snake.snake_len > 0){
        snake.snake_body[0][0] = snake.snake_head[0];
        snake.snake_body[0][1] = snake.snake_head[1];
    }
    // move head
    if(dir == UP){
        if(snake.snake_head[0] == 0){
            snake.snake_head[0] = 7;
        }
        else{
            snake.snake_head[0] -= 1;
        }
    }else if(dir == DOWN){
        snake.snake_head[0] = (snake.snake_head[0] + 1)%8;
    }else if(dir == LEFT){
        if(snake.snake_head[1] == 0){
            snake.snake_head[1] = 7;
        }
        else{
            snake.snake_head[1] -= 1;
        }
    }else if(dir == RIGTH){
        snake.snake_head[1] = (snake.snake_head[1] + 1)%8;
    }
    if(apple_head_collision()){
        snake.snake_body[snake.snake_len][0] = temp_last_row;
        snake.snake_body[snake.snake_len][0] = temp_last_col;
        snake.snake_len++;
        gen_apple();
        printf("snake len %d\n", snake.snake_len);
    }
    else if(head_body_collision()){
        reset_snake();
    }
    if(snake.snake_len >= 64){
        reset_snake();
    }
    print_snake();
    return;
}


int main(int argc, char *argv[]) {
    char c;
    reset_snake();
    gen_apple();    
    refresh();
    while(1){
        c = getchar();
        if(c == 'e'){
            break;
        }
        switch(c){
            case 'u':
            dir = UP;
            refresh();
            break;
            case 'd':
            dir = DOWN;
            refresh();
            break;
            case 'l':
            dir = LEFT;
            refresh();
            break;
            case 'r':
            dir = RIGTH;
            refresh();
            break;
        }
        
    }
    return 0;
}