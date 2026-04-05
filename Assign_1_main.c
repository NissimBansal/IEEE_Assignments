#include <stdint.h>

#define AHB1_RCC 0x40023800
#define AHB1_GPIOA 0x40020000
#define APB1_TIM2 0x40000000

void TIM2_IRQHandler(void);

int main(void)
{
	*((uint32_t*)(AHB1_RCC + 0x30)) |= (1 << 0); /* enabling clock for GPIOA */
	*((uint32_t*)(AHB1_RCC + 0x40)) |= (1 << 0); /* enabling clock for TIM2 */
	*((uint32_t*)(AHB1_GPIOA)) |= (1 << 10); /* setting MODER5 to output */
	*((uint32_t*)(AHB1_GPIOA)) &= ~(1 << 11); /* setting MODER5 to output */
	*((uint32_t*)(APB1_TIM2 + 0x28)) = 8399; /* setting prescalar */
	*((uint32_t*)(APB1_TIM2 + 0x2C)) = 19999; /* setting auto-reload */
	*((uint32_t*)(APB1_TIM2 + 0x0C)) |= (1 << 0); /* enabling DIER / interrupt channel */
	*((volatile uint32_t*)(0xE000E100)) |= (1 << 28); /*  NVIC now responds to TIM2_DIER changes*/
	*((uint32_t*)(APB1_TIM2)) |= (1 << 0); /* starts counter */

	while(1)
	{
		__asm volatile ("wfi"); /* assembly code that makes cpu sleep until a flag is passed */
	}
}

void TIM2_IRQHandler(void)
{
	*((uint32_t*)(APB1_TIM2 + 0x10)) &= ~(1 << 0); /* resets the flag */
	*((uint32_t*)(AHB1_GPIOA + 0x14)) ^= (1 << 5); /* toggles LED between ON & OFF */
}