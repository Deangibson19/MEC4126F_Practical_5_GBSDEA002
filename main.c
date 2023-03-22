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
void init_external_interrupts(void);

uint8_t SW3_count = 0;


// MAIN FUNCTION -------------------------------------------------------------|
void main(void)
{
	init_LEDs();
	init_LCD();
	init_switches();
	init_external_interrupts();

	uint8_t count = 0;

	while(1)
	{
		if ((SW3_count%2) != 0)
		{
			if ((GPIOA->IDR & GPIO_IDR_1) == 0)
			{
				if (count < 255)
				{
					count++;
				}
				//else
				//{
					//count = count;
				//}
				lcd_command(CLEAR);
				display_on_LCD(count);
				display_on_LEDs(count);
				delay(50000);
			}
			else if ((GPIOA->IDR & GPIO_IDR_2) == 0)
			{
				if (count <= 0)
				{
					count = count;
				}
				else
				{
					count--;
				}
				lcd_command(CLEAR);
				display_on_LCD(count);
				display_on_LEDs(count);
				delay(50000);
			}
		}
		else
		{
			count = 0;
			lcd_command(CLEAR);
			display_on_LEDs(count);
		}

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

	GPIOA -> MODER &= ~GPIO_MODER_MODER1;
	GPIOA -> MODER &= ~GPIO_MODER_MODER2;
	GPIOA -> MODER &= ~GPIO_MODER_MODER3;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR1_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR2_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR3_0;
}

//const int COUNT_MIN = 0;
//const int COUNT_MAX = 255;

//int update_count(int count)
//{
//    // increment count if SW1 is pressed and count is less than COUNT_MAX
//    if((( (GPIOA -> IDR) &= GPIO_IDR_1)==0) && count < COUNT_MAX)
//    {
//        (count++);
//    }
//    // decrement count if SW2 is pressed and count is greater than COUNT_MIN
//    if((( (GPIOA -> IDR) &= GPIO_IDR_2)==0) && count > COUNT_MIN)
//    {
//        (count)--;
//    }
//    if((( (GPIOA -> IDR) &= GPIO_IDR_3)==0) && count < 255)
//       {
//           count = count;
//       }
//    return count;
//}

void init_external_interrupts(void)
{
	 RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

	    SYSCFG->EXTICR[2] |= SYSCFG_EXTICR1_EXTI3_PA;
	    EXTI->IMR |= EXTI_IMR_MR3;
	    EXTI->FTSR |= EXTI_FTSR_TR3;

	    NVIC_EnableIRQ(EXTI2_3_IRQn);
}

void EXTI2_3_IRQHandler(void)
{
	SW3_count +=1;
	EXTI -> PR |= EXTI_PR_PR3;
	delay(50000);

}



