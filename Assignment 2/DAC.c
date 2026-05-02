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
}
