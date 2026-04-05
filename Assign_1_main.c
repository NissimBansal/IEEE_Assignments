#include <stdint.h>

#define AHB1_RCC 0x40023800
#define AHB1_GPIOA 0x40020000
#define APB1_TIM2 0x40000000

int main(void)
{
	*((uint32_t*)(AHB1_RCC + 0x30)) |= (1 << 0); /* enabling clock for GPIOA */
	*((uint32_t*)(AHB1_RCC + 0x40)) |= (1 << 0); /* enabling clock for TIM2 */
	*((uint32_t*)(AHB1_GPIOA)) |= (1 << 10); /* setting MODER5 to output part 1 */
	*((uint32_t*)(AHB1_GPIOA)) &= ~(1 << 11); /* setting MODER5 to output part 2*/
	*((uint32_t*)(APB1_TIM2 + 0x28)) = 8399; /* setting prescalar */
	*((uint32_t*)(APB1_TIM2 + 0x2C)) = 19999; /* setting auto-reload */
}
