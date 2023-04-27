#include <LedControl.h>

int DIN = 11;
int CS = 7;
int CLK = 13;

LedControl lc=LedControl(DIN, CLK, CS,0);

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGTH 3
#define NO_DATA 12 // Some random number > 7

int apple[2] = {NO_DATA, NO_DATA};
byte Apple [8]={B00011000,B00001000,B01110110,B11111111,B11111111,B11111111,B01111010,B00110100};

struct Snake {
    int snake_body[64][2];
    int snake_head[2];
    int snake_len;
};
typedef struct Snake Snake;

Snake snake;

volatile int dir = UP;

unsigned long framerate_counter = 0;
unsigned int frame_duration = 200;
unsigned long button_time = 0;
unsigned int btn_delay = 50;
int dir_changed = false;
int num_attempts = 5;


void reset_snake(void){
    for (int i = 0; i < 64; ++i){
            for(int j = 0; j< 2; ++j){
                snake.snake_body[i][j] = NO_DATA;
            }
        }
        snake.snake_head[0] = random(0, 8);// random(0, 8) in ArduinoIDE
        snake.snake_head[1] = random(0, 8);
        snake.snake_len = 0;
}

void gen_apple(void){
    int a_row = random(0, 8);
    int a_col = random(0, 8);
    int good_apple_pos = 0;
    
    while(!good_apple_pos){
        int apple_in_body = 0;
        for(int i = 0; i < snake.snake_len; ++i){
            if(snake.snake_body[i][0] == a_row && snake.snake_body[i][1] == a_col){
                apple_in_body = 1;
                break;
            }
        }
        if ((a_row == snake.snake_head[0] && a_col == snake.snake_head[1]) || apple_in_body){
            a_row = random(0, 8);
            a_col = random(0, 8);
        }
        else{
            good_apple_pos = 1;
        }
    }
    apple[0] = a_row;
    apple[1] = a_col;
}

int  apple_head_collision(){
    if (apple[0] == snake.snake_head[0] && apple[1] == snake.snake_head[1]){
        return 1;
    }
    else{
        return 0;
    }
}

int head_body_collision(){
    for(int i = 0; i < snake.snake_len; ++i){
        if(snake.snake_head[0] == snake.snake_body[i][0] && snake.snake_head[1] == snake.snake_body[i][1]){
            return 1;
        }
    }
    return 0;
}

int is_snake(int row, int col){
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


void draw_snake(){
  lc.clearDisplay(0);
  lc.setLed(0, snake.snake_head[1], 7 - snake.snake_head[0], true);
  lc.setLed(0, apple[1], 7 - apple[0], true);
  for(int l = 0; l<snake.snake_len; ++l){
    lc.setLed(0, snake.snake_body[l][1], 7 - snake.snake_body[l][0], true);
  }
}


void refresh(void){
    Serial.println(dir);
    int temp_last_row = 0;
    int temp_last_col = 0;
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
    }
    else if(head_body_collision()){
        reset_snake();
    }
    if(snake.snake_len >= 64){
        reset_snake();
    }
    draw_snake();
    return;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(12, INPUT); 
  reset_snake();
  gen_apple();

  lc.shutdown(0,false);
  lc.setIntensity(0,3);
  lc.clearDisplay(0);

  PCICR |= B00000100;
  // Select PCINT23 Bit7 = 1 (Pin D7)
  PCMSK2 |= B00010111;
  for(int i = 0; i< 8; ++i){
    lc.setRow(0,i,Apple[i]);                                                                                                               
  }
  delay(1000);
}


void loop() {
  // put your main code here, to run repeatedly:
  
  if((millis() - framerate_counter) > frame_duration){
    framerate_counter = millis();
    refresh();
  }
  delay(1);

}

ISR (PCINT2_vect)
{
  if((millis() - button_time) > btn_delay){
    for(int i = 0; i < num_attempts; ++i){
      if(digitalRead(8) == HIGH){
        dir = DOWN;
        button_time = millis();
        dir_changed = true;
      }
      else if(digitalRead(9) == HIGH){
        dir = LEFT;
        button_time = millis();
        dir_changed = true;
      }
      else if(digitalRead(10) == HIGH){
        dir = UP;
        button_time = millis();
        dir_changed = true;
      }
      else if(digitalRead(12) == HIGH){
        dir = RIGTH;
        button_time = millis();
        dir_changed = true;
      }
      if(dir_changed){
        break;
      }
    }
    dir_changed = false;
  }
}
