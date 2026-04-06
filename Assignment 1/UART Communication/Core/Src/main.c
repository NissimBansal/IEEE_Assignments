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

#include <stm32f4xx.h>
#include <stdint.h>

#define AHB1_RCC 0x40023800
#define AHB1_GPIOA 0x40020000
#define APB1_USART2 0x40004400

static void send_byte(uint8_t b)
{
    while (!(USART2->SR & USART_SR_TXE)); /* keep checking SR and TXE until TXE becomes 1, then DR executes */
    USART2->DR = b; /* writing one byte to data register */
}

static void send_string(const char *s)
{
    while (*s) send_byte((uint8_t)*s++); /* keeps sending data to RealTerm byte-wise */
}


int main(void)
{
	*((volatile uint32_t*)(AHB1_RCC + 0x30)) |= (1 << 0); /* enabling clock for GPIOA */
	*((volatile uint32_t*)(AHB1_RCC + 0x40)) |= (1 << 17); /* enabling clock for USART2 */

	*((volatile uint32_t*)(AHB1_GPIOA)) &= ~(1 << 4); /* setting PA2 to AF mode */
	*((volatile uint32_t*)(AHB1_GPIOA)) |= (1 << 5);  /* setting PA2 to AF mode */
	*((volatile uint32_t*)(AHB1_GPIOA)) &= ~(1 << 6); /* setting PA3 to AF mode */
	*((volatile uint32_t*)(AHB1_GPIOA)) |= (1 << 7);  /* setting PA3 to AF mode */
	*((volatile uint32_t*)(AHB1_GPIOA + 0x20)) |= (1 << 8); /* setting PA2 to AF7 or USART */
	*((volatile uint32_t*)(AHB1_GPIOA + 0x20)) |= (1 << 9); /* setting PA2 to AF7 or USART */
	*((volatile uint32_t*)(AHB1_GPIOA + 0x20)) |= (1 << 10); /* setting PA2 to AF7 or USART */
	*((volatile uint32_t*)(AHB1_GPIOA + 0x20)) &= ~(1 << 11); /* setting PA2 to AF7 or USART */
	*((volatile uint32_t*)(AHB1_GPIOA + 0x20)) |= (1 << 12); /* setting PA3 to AF7 or USART */
	*((volatile uint32_t*)(AHB1_GPIOA + 0x20)) |= (1 << 13); /* setting PA3 to AF7 or USART */
	*((volatile uint32_t*)(AHB1_GPIOA + 0x20)) |= (1 << 14); /* setting PA3 to AF7 or USART */
	*((volatile uint32_t*)(AHB1_GPIOA + 0x20)) &= ~(1 << 15); /* setting PA3 to AF7 or USART */

	*((volatile uint32_t*)(APB1_USART2 + 0x08)) = 0x008B; /* setting baud rate register divisor = 8.6085 */
	*((volatile uint32_t*)(APB1_USART2 + 0x0C)) |= (1 << 2); /* enabling RX */
	*((volatile uint32_t*)(APB1_USART2 + 0x0C)) |= (1 << 3); /* enabling TX */
	*((volatile uint32_t*)(APB1_USART2 + 0x0C)) |= (1 << 13); /* enabling USART */

	send_string("Never gonna give you up");

    while (1) {}
}
