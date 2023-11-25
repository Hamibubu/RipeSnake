#include "ripes_system.h"
#define SW0 (0x01)
#define SW1 (0x02)
#define SW2 (0x04)
#define SW3 (0x08)
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
while(1)
{
    //Por siempre leemos que flecha se movió y añadimos acorde esa posición en nuestra base
        //Aquí luego tenemos que estar borrando los leds "antiguos" espacios donde estaba la serpiente y ya no
    if(*d_pad_up == 1) led_base -= LED_MATRIX_0_WIDTH;
    if(*d_pad_do == 1) led_base += LED_MATRIX_0_WIDTH;
    if(*d_pad_le == 1) led_base -= 1;
    if(*d_pad_ri == 1) led_base += 1;

    //coloreamos los leds
    *led_base = 0xFFD700;
}
}