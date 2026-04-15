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

void Initialize()
{
	RCC -> AHB1ENR |= (1 << 0); /* Clock for GPIOA */
	RCC -> APB1ENR |= (1 << 17); /* Clock for UART */
	RCC -> APB2ENR |= (1 << 8); /* Clock for ADC */

	GPIOA -> MODER |= (3 << 2); /* PA1 to analog mode */
	GPIOA -> MODER &= ~(1 << 4); /* PA2 to AF mode */
	GPIOA -> MODER |= (1 << 4); /* PA2 to AF mode */
	GPIOA -> AFR[0] &= ~(1 << 11); /* PA2 to AF7 or USART2_TX */
	GPIOA -> AFR[0] |= (1 << 10); /* PA2 to AF7 or USART2_TX */
	GPIOA -> AFR[0] |= (1 << 9); /* PA2 to AF7 or USART2_TX */
	GPIOA -> AFR[0] |= (1 << 8); /* PA2 to AF7 or USART2_TX */

	ADC -> CCR &= ~(1 << 17); /* Prescalar for ADC to receive 4 MHz clock */
	ADC -> CCR |= (1 << 16); /* Prescalar for ADC to receive 4 MHz clock */
	ADC1 -> CR1 &= ~(3 << 24); /* Resolution to 12-bit */
	ADC1 -> CR1 &= ~(1 << 8); /* Scan mode = 0 i.e. convert one channel and stop*/
	ADC1 -> CR2 &= ~(1 << 1); /* Single conversion mode */
	ADC1 -> CR2 &= ~(1 << 11); /* Right alignment */
	ADC1 -> CR2 &= ~(3 << 28); /* External trigger to ADC disabled */
	ADC1 -> SQR3  |=  (1 << 0);	/* First channel to convert is channel 1 */
	ADC1 -> SQR1 &= ~(15 << 20); /* Only 1 conversion */
	ADC1 -> SMPR2 |= (7 << 3); /* Sampling time to 480 cycles */
	ADC1 -> CR2 |= (1 << 0); /* Power on ADC */
	for (volatile uint32_t i = 0; i < 320; i++) __NOP(); /* To give time to internal circuits to stabilize  */

	USART2 -> BRR = 0x008B; /* Baud rate set */
	USART2 -> CR1 |= (1<< 13); /* USART2 on */
	USART2 -> CR1 |= (1<< 3); /* Transmitter on */
	USART2 -> CR1 |= (1<< 2); /* Receiver on */
}

int main()
{
	Initialize();

	uint16_t last_DAC_Value = 4500; /* A value impossible to have in ADC */

	while (1)
	{
		uint16_t DAC_Value = ADC_Reading();

		int32_t delta = (int32_t)DAC_Value - (int32_t)last_DAC_Value;

		if ((delta > 8) || (delta < -8)) /* this is approx 6.4 mV, good to check if knob was rotated but not because of noise */
		{

		}
	}
}

uint16_t ADC_Reading()
{
	ADC1 -> CR2 |= (1 << 30); /* Take a single sample */
	while (!(ADC1 -> DR && ADC_SR_EOC)); /* Keep looping while taking sample */
	return (uint16_t)ADC1 -> DR; /* Reading ADC_DR resets EOC to 1 */
}

