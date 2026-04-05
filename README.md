# Assignment 1

## Part 1: Timer-Based LED Control

### Introduction to Timer Peripheral in Microcontrollers
Timer peripherals are hardware components that implement the working of a timer. A timer is a simply counter that works by itself driven by a clock. It can track time, measure external pulse widths/signals, trigger events (like ADC and DAC conversions) without any help from the CPU.
### Why are hardware timers preferred over software delay functions? 
A software timer takes up unnecessary memory in CPU that could otherwise be used by other functions. Instead hardware timers do the counting by themselves silently in the background. While performing a software delay function, the CPU cant do anything else and has to wait for it to finish. Due to many hardware timers present in the board, you can use different timers to carry out different processes at the same time. If there is some other code between the iterations of delay, that time also gets added which is not a problem in hardware timers. 
### Basic working concepts of timers
The registers used in achieving a bliinking LED are as follows-  
1. 