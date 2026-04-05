# Assignment 1

## Part 1: Timer-Based LED Control

### Introduction to Timer Peripheral in Microcontrollers
Timer peripherals are hardware components that implement the working of a timer. A timer is a simply counter that works by itself driven by a clock. It can track time, measure external pulse widths/signals, trigger events (like ADC and DAC conversions) without any help from the CPU.

### Why are hardware timers preferred over software delay functions? 
A software timer takes up unnecessary memory in CPU that could otherwise be used by other functions. Instead, hardware timers do the counting by themselves silently in the background. While performing a software delay function, the CPU can't do anything else and has to wait for it to finish. Due to many hardware timers present in the board, you can use different timers to carry out different processes at the same time. If there is some other code between the iterations of delay, that time also gets added which is not a problem in hardware timers. 

### Basic working concepts of timers
- Tick : The system clock oscillates at a fixed frequency and each complete oscillation is sent as a signal/pulse which is one tick. The APB1 bus (which is where our timer is) receives a clock signal of 84 MHz per second.
- Counter : The counter is the heart of the timer and is used as a unit which helps trigger overflow events and such. It increases by one every tick. 
- Prescalar : It is a value which the incoming clock signal from system clocks is divided by before it reaches the counter. The formula is Frequency (counter) = Frequency (System) / [Prescalar value + 1].
- Auto reload : It is also a value that defines the max value the counter can count upto before it resets and interrupt flag is sent.
- Interrupt file : It is a signal sent to the CPU which wakes it up and ultimately leads it functions like toggling LED, clearing flag and resetting counter.

### Register Configuration for 2-Second Delay

#### Choice of Timer
TIM2 was chosen as it's a general purpose timer and it was really easy to configure the clock and other registers for it. 

#### Prescalar value
The PSC value chosen is 8399 as when we add 1, it becomes 8400 and 84 MHz divided by 8400 gives 10k ticks per second or 1 tick every 0.1 ms. This round number helps make further calculations easier.

#### ARR calculation
Since we want the LED to toggle every 2 seconds, this implies, ARR = (2s / 0.1 ms) - 1. I added -1 as ARR starts counting from 0. This gives ARR = 19,999.

#### Control and Status Registers
- Control enable register : It is used to start the counter.
- Status register : It generates an interrupt by setting itself to 1 whenever counter overflows ARR.
- DMA/Interrupt enable register : It allows the update/interrupt to go towards NVIC (Nested vectored interrupt controller), otherwise an overflowing timer doesn't mean anything.
- Nested vectored interrupt controller : Used to enable the line through which the interrupt is sent to the CPU.
- GPIOA ODR : There is an XOR gate connected to this register that toggles the LED every 2 seconds.

#### How the update flag is used for timing
1. First the counter goes upto the value set by auto-reload register and when it reaches that value it sets the status register automatically to 1 (flag).
2. This is then sent by a channel enabled via DIER and goes through NVIC to CPU.
3. The CPU carries out TIM2_IRQHandler function which is already defined but we can write it differently in our .c file which resets flag status to 0 and toggles LED.
4. The counter is counting independently starting the cycle again.