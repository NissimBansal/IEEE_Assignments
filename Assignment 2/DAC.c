/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "stm32f446xx.h"
#include <stdint.h>

void init(void);
void delay_us(volatile uint32_t us);

void init(void)
{
	RCC -> AHB1ENR |= (1 << 0); /* Clock for GPIOA */
	RCC -> APB1ENR |= (1 << 0); /* Clock for DAC */

	GPIOA -> MODER |= (1 << 8); /* PA4 to Analog mode */
	GPIOA -> MODER |= (1 << 9); /* PA4 to Analog mode */

	DAC -> CR |= (1 << 0); /* Enable DAC channel 1 */
	DAC -> CR &= ~(1 << 2); /* Disable external trigger for Digital-Analog conversion */

}

int main()
{
	init();

	while (1)
	{
		for (uint16_t value = 0; value <= 4095; value++) /* Code to generate sawtooth wave */
		{
			DAC -> DHR12R1 = value; /* put the value into DAC holding register */
			delay_us(4); /* 4 microseconds of t(settling) for data to go into DOR */
		}
	}
}

void delay_us(volatile uint32_t us)
{
	while (us--)
	{
		for (volatile int i = 0; i < 16; i++) __NOP(); /* To give 1 microseconds of delay */
	}
}
