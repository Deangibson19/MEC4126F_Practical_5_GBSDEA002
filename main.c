#define STM32F051 												   //COMPULSORY
#include "stm32f0xx.h"											   //COMPULSORY
#include "lcd_stm32f0.h"
#include <stdio.h>


// FUNCTION DECLARATIONS -----------------------------------------------------|

void main(void);
void lcd_putstring(char * instring);
void display_on_LCD(unsigned char count);
void init_LEDs(void);
void display_on_LEDs(uint8_t count);
void init_switches(void);
void delay(unsigned int microseconds);
int update_count(int count);



// MAIN FUNCTION -------------------------------------------------------------|

void main(void)
{
	init_LEDs();
	init_LCD();
	init_switches();

	int count = 0;

	while(1)
	{
		count= update_count(count);
		delay(20000);
		display_on_LCD(count);
		display_on_LEDs(count);
	}

}


// OTHER FUNCTIONS -----------------------------------------------------------|
void display_on_LCD(unsigned char count)
{
	lcd_command(CLEAR);
	char str[4];
	sprintf(str,"%d", count);
	lcd_putstring(str);
	delay(20000);

}



void init_LEDs(void)
{
	RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB -> MODER |= GPIO_MODER_MODER0_0;
	GPIOB -> MODER |= GPIO_MODER_MODER1_0;
	GPIOB -> MODER |= GPIO_MODER_MODER2_0;
	GPIOB -> MODER |= GPIO_MODER_MODER3_0;
	GPIOB -> MODER |= GPIO_MODER_MODER4_0;
	GPIOB -> MODER |= GPIO_MODER_MODER5_0;
	GPIOB -> MODER |= GPIO_MODER_MODER6_0;
	GPIOB -> MODER |= GPIO_MODER_MODER7_0;

}

void display_on_LEDs(uint8_t count){GPIOB->ODR = count;}


void init_switches(void)
{
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

	GPIOA -> MODER &= ~GPIO_MODER_MODER0;
	GPIOA -> MODER &= ~GPIO_MODER_MODER1;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR0_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR1_0;
}

const int COUNT_MIN = 0;
const int COUNT_MAX = 255;

int update_count(int count)
{
    // increment count if SW1 is pressed and count is less than COUNT_MAX
    if((( (GPIOA -> IDR) &= GPIO_IDR_0)==0) && count < COUNT_MAX)
    {
        (count++);
    }
    // decrement count if SW2 is pressed and count is greater than COUNT_MIN
    if((( (GPIOA -> IDR) &= GPIO_IDR_1)==0) && count > COUNT_MIN)
    {
        (count)--;
    }
    return count;
}





