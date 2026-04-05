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

#include <stdint.h>

#define AHB1_RCC 0x40023800
#define AHB1_GPIOA 0x40020000
#define APB1_TIM2 0x40000000

int main(void)
{
	*((volatile uint32_t*)(AHB1_RCC + 0x30)) |= (1 << 0); /* enabling clock for GPIOA */
	*((volatile uint32_t*)(AHB1_RCC + 0x40)) |= (1 << 0); /* enabling clock for TIM2 */
	*((volatile uint32_t*)(AHB1_GPIOA)) |= (1 << 10); /* setting MODER5 to output */
	*((volatile uint32_t*)(AHB1_GPIOA)) &= ~(1 << 11); /* setting MODER5 to output */
	*((volatile uint32_t*)(APB1_TIM2 + 0x28)) = 1599; /* setting prescalar */
	*((volatile uint32_t*)(APB1_TIM2 + 0x2C)) = 19999; /* setting auto-reload */
	*((volatile uint32_t*)(APB1_TIM2 + 0x0C)) |= (1 << 0); /* enabling DIER / interrupt channel */
	*((volatile uint32_t*)(0xE000E100)) |= (1 << 28); /*  NVIC now responds to TIM2_DIER changes*/
	*((volatile uint32_t*)(APB1_TIM2)) |= (1 << 0); /* starts counter */

	while(1)
	{
		__asm volatile ("wfi"); /* assembly code that makes CPU sleep until a flag is passed */
	}
}

void TIM2_IRQHandler(void)
{
	*((volatile uint32_t*)(APB1_TIM2 + 0x10)) &= ~(1 << 0); /* resets the flag */
	*((volatile uint32_t*)(AHB1_GPIOA + 0x14)) ^= (1 << 5); /* toggles LED between ON & OFF */
}
