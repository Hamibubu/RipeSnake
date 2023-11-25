#include "ripes_system.h"
#include <time.h>
#include <stdlib.h>


#define SW0 (0x01)
#define SW1 (0x02)
#define SW2 (0x04)
#define SW3 (0x08)
#define LED_MATRIX_0_SIZE	(0xdac)
#define LED_MATRIX_0_WIDTH	(0x23)
#define LED_MATRIX_0_HEIGHT	(0x19)
//apuntador a la matriz de leds
volatile unsigned int * led_base = LED_MATRIX_0_BASE;
//Inputs
volatile unsigned int * d_pad_up = D_PAD_0_UP;
volatile unsigned int * d_pad_do = D_PAD_0_DOWN;
volatile unsigned int * d_pad_le = D_PAD_0_LEFT;
volatile unsigned int * d_pad_ri = D_PAD_0_RIGHT;
void main()
{
unsigned int mask = 0;

    //Area de la matriz a usar, menos 2 para tener un espacio donde no se salga la manzana
    int matrixArea = (LED_MATRIX_0_HEIGHT-2) * (LED_MATRIX_0_SIZE-2);
    srand(time(NULL));   // Initialization, should only be called once.
    int r = rand() % (matrixArea +1);      // Returns a pseudo-random integer between 0 and matrixArea.
    //Se genera la manzana
    //
    //  . .     a a+1
    //  . .     r r+1
    //
    led_base = r;   //r
    led_base += 1 ; //r+1
    led_base = LED_MATRIX_0_WIDTH - r ;  //salto de linea atrás "a"
    led_base +=  1; //a+1



    //coloreamos los leds
    *led_base = 0xFFD700;

}