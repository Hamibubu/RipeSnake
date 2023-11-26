#include "ripes_system.h"
#include <stdio.h>

#define SW0 (0x01)
#define SW1 (0x02)
#define SW2 (0x04)
#define SW3 (0x08)
#define SW4 (0x10)
#define SW5 (0x20)
#define SW6 (0x40)
#define SW7 (0x80)

#define LED_MATRIX_0_SIZE	(0xdac)
#define LED_MATRIX_0_WIDTH	(0x23)
#define LED_MATRIX_0_HEIGHT	(0x19)

volatile unsigned int * led_base = (volatile unsigned int *)LED_MATRIX_0_BASE;
volatile unsigned int * d_pad_up = (volatile unsigned int *)D_PAD_0_UP;
volatile unsigned int * d_pad_down = (volatile unsigned int *)D_PAD_0_DOWN;
volatile unsigned int * d_pad_left = (volatile unsigned int *)D_PAD_0_LEFT;
volatile unsigned int * d_pad_right = (volatile unsigned int *)D_PAD_0_RIGHT;
volatile unsigned int * switch_base = (volatile unsigned int *)SWITCHES_0_BASE;

#define MAX_SNAKE_SIZE (LED_MATRIX_0_WIDTH * LED_MATRIX_0_HEIGHT)

int head = 1;
int tail = 0;

int size = 2;

int dx = 0;
int dy = 0;

volatile unsigned int snakeLEDs[MAX_SNAKE_SIZE];

void initSnake() {
    int st1 = (LED_MATRIX_0_WIDTH+2);
    int st2 = (LED_MATRIX_0_WIDTH*2+2);
    *(led_base+st1) = 0x00FF00;
    *(led_base+st2) = 0x00FF00;
    snakeLEDs[tail] = st1;
    snakeLEDs[head] = st2;
}

void moveSnake() {
    int new_head_index = (snakeLEDs[head] + LED_MATRIX_0_WIDTH);
    head = (head + 1) % MAX_SNAKE_SIZE;
    snakeLEDs[head] = new_head_index;
    *(led_base + new_head_index) = 0x00FF00;
    *(led_base + snakeLEDs[tail]) = 0x0;
    tail = (tail + 1) % MAX_SNAKE_SIZE;
}

void printlimits(int color) {
    for (int i = 0; i < LED_MATRIX_0_WIDTH+1; i++)
    {
        *(led_base+i)=color;
        *(led_base+(LED_MATRIX_0_HEIGHT-1) * LED_MATRIX_0_WIDTH + i) = color;
    }
    for (int i = 0; i < LED_MATRIX_0_HEIGHT; i++) {
        *(led_base + i * LED_MATRIX_0_WIDTH) = color; 
        *(led_base + i * LED_MATRIX_0_WIDTH-1) = color;
    }
}

void main() {
    initSnake();
    while (1){
        moveSnake();
    }
}