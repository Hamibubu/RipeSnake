#include "ripes_system.h"
#include <stdio.h>
#include <stdlib.h>

#define SW0 (0x01)
#define SW1 (0x02)
#define SW2 (0x04)
#define SW3 (0x08)
#define SW4 (0x10)
#define SW5 (0x20)
#define SW6 (0x40)
#define SW7 (0x80)

// Constantes de tamaños
#define LED_MATRIX_0_SIZE	(0xdac)
#define LED_MATRIX_0_WIDTH	(0x23)
#define LED_MATRIX_0_HEIGHT	(0x19)

// Apuntadores a diferentes partes importantesm base y dpad
volatile unsigned int * led_base = (volatile unsigned int *)LED_MATRIX_0_BASE;

volatile unsigned int * d_pad_up = (volatile unsigned int *)D_PAD_0_UP;
volatile unsigned int * d_pad_do = (volatile unsigned int *)D_PAD_0_DOWN;
volatile unsigned int * d_pad_le = (volatile unsigned int *)D_PAD_0_LEFT;
volatile unsigned int * d_pad_ri = (volatile unsigned int *)D_PAD_0_RIGHT;

// Tamaño máximo
#define MAX_SNAKE_SIZE (LED_MATRIX_0_WIDTH * LED_MATRIX_0_HEIGHT)

// Variables iniciales
int game = 1;
int head = 1;
int tail = 0;

int randcounter = 50;

int size = 2;

int new_head_index;

int dx = 0;
int dy = 1;

// Arreglo donde se guardan las head y tails
volatile unsigned int snakeLEDs[MAX_SNAKE_SIZE];

void initSnake() {
    int st1 = (LED_MATRIX_0_WIDTH+2);
    int st2 = (LED_MATRIX_0_WIDTH*2+2);
    *(led_base+st1) = 0x00FF00;
    *(led_base+st2) = 0x00FF00;
    snakeLEDs[tail] = st1;
    snakeLEDs[head] = st2;
}

// Mover snake
int moveSnake() {
    // Verificar dirección
    if (dy == 1) {
        new_head_index = (snakeLEDs[head] + LED_MATRIX_0_WIDTH);
    } else if (dy == -1) {
        new_head_index = (snakeLEDs[head] - LED_MATRIX_0_WIDTH);
    } else if (dx == 1) {
        new_head_index = (snakeLEDs[head] + 1);
    } else if (dx == -1) {
        new_head_index = (snakeLEDs[head] -1);
    }
    // Colisiones para que acabe el juego
    if (*(led_base+new_head_index)==0xF9F6B9 || *(led_base+new_head_index)==0x00FF00 ) {
        return 0;
    } else if (*(led_base+new_head_index)==0xFF0000) {
        eatApple();   // Colision con la manzana = comer
        generateApple();
        return 1;
    } // Nuevo espacio en array de la head
    head = (head + 1) % MAX_SNAKE_SIZE; 
    snakeLEDs[head] = new_head_index; // La guardamos en el array
    *(led_base + new_head_index) = 0x00FF00;
    *(led_base + snakeLEDs[tail]) = 0x0; // Apagar tail
    tail = (tail + 1) % MAX_SNAKE_SIZE; // Nuevo espacio de la tail
    return 1;
}

// Imprimir los bordes
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

// Apagar todos los leds
void cleanBoard() {
    for (int i = 0; i < MAX_SNAKE_SIZE; i++) {
        *(led_base+i) = 0x0;
    }
}

// Cambiar direcciones para la función moveSnake
void changeDirection(int dex, int dey) {
    dx = dex;
    dy = dey;
}

// Que pasa al comer la manzana
void eatApple() {
    head = (head + 1) % MAX_SNAKE_SIZE;
    snakeLEDs[head] = new_head_index;
    *(led_base + new_head_index) = 0x00FF00;
}

// Generar aleatoriamente la manzana
void generateApple() {
    int flag = 1;
    while (flag){
        randcounter += 5;
        srand(randcounter);
        int random_x = 2 + rand() % (LED_MATRIX_0_WIDTH -4);
        int random_y = 2 + rand() % (LED_MATRIX_0_HEIGHT -4);
        int position = random_y * LED_MATRIX_0_WIDTH + random_x;
        if (*(led_base+position) != 0xF9F6B9 || *(led_base+position) != 0x00FF00) {
            *(led_base+position) = 0xFF0000;
            flag = 0;
        }
    }
}

void main() {
    cleanBoard();
    initSnake();
    printlimits(0xF9F6B9);
    generateApple();
    while (game){
        game = moveSnake();
        for (int i = 0; i < 10000; i++) {
            
        }
        // Movimientos del dpad
        if(*d_pad_up == 1) changeDirection(0, -1);
        if(*d_pad_do == 1) changeDirection(0, 1);
        if(*d_pad_le == 1) changeDirection(-1, 0);
        if(*d_pad_ri == 1) changeDirection(1, 0); 
    }
    cleanBoard();
}