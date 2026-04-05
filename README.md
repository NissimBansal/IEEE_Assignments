# Assignment 1

## Part 1: Timer-Based LED Control

### Introduction to Timer Peripheral in Microcontrollers
Timer peripherals are hardware components that implement the working of a timer. A timer is a simply counter that works by itself driven by a clock. It can track time, measure external pulse widths/signals, trigger events (like ADC and DAC conversions) without any help from the CPU.
### Why are hardware timers preferred over software delay functions? 
A software timer takes up unnecessary memory in CPU that could otherwise be used by other functions. Instead hardware timers do the counting by themselves silently in the background. While performing a software delay function, the CPU cant do anything else and has to wait for it to finish. Due to many hardware timers present in the board, you can use different timers to carry out different processes at the same time. If there is some other code between the iterations of delay, that time also gets added which is not a problem in hardware timers. 
### Basic working concepts of timers

- Tick : The system clock oscillates at a fixed frequency and each complete oscillation is sent as a signal/pulse which is one tick. The APB1 bus (which is where our timer is) receives a clock signal of 84 MHz per second.
- Counter : The counter is the heart of the timer and is used as a unit which helps trigger overflow events and such. It increases by one every tick. 
- Prescalar : It is a value which the incoming clock signal from system clocks is divided by before it reaches the counter. The formula is Frequency(counter) = Frequency(System)/[Prescalar value + 1].
- Auto reload : It is also a value that defines the max value the counter can count upto before it resets and and interrupt flag is sent.