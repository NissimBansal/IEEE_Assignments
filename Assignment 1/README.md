## Part 1: Timer-Based LED Control

### Introduction to Timer Peripheral in Microcontrollers
Timer peripherals are hardware components that implement the working of a timer. A timer is a simply counter that works by itself driven by a clock. It can track time, measure external pulse widths/signals, trigger events (like ADC and DAC conversions) without any help from the CPU.

### Why are hardware timers preferred over software delay functions? 
A software timer takes up unnecessary memory in CPU that could otherwise be used by other functions. Instead, hardware timers do the counting by themselves silently in the background. While performing a software delay function, the CPU can't do anything else and has to wait for it to finish. Due to many hardware timers present in the board, you can use different timers to carry out different processes at the same time. If there is some other code between the iterations of delay, that time also gets added which is not a problem in hardware timers. 

### Basic working concepts of timers
- Tick : The system clock oscillates at a fixed frequency and each complete oscillation is sent as a signal/pulse which is one tick. The APB1 bus (which is where our timer is) receives a clock signal of 16 MHz.
- Counter : The counter is the heart of the timer and is used as a unit which helps trigger overflow events and such. It increases by one every tick. 
- Prescalar : It is a value which the incoming clock signal from system clocks is divided by before it reaches the counter. The formula is Frequency (counter) = Frequency (System) / [Prescalar value + 1].
- Auto reload : It is also a value that defines the max value the counter can count upto before it resets and interrupt flag is sent.
- Interrupt file : It is a signal sent to the CPU which wakes it up and ultimately leads it functions like toggling LED, clearing flag and resetting counter.

### Register Configuration for 2-Second Delay

#### Choice of Timer
TIM2 was chosen as it's a general purpose timer and it was really easy to configure the clock and other registers for it. 

#### Prescalar value
The PSC value chosen is 1599 as when we add 1, it becomes 1600 and 16 MHz divided by 1600 gives 10k ticks per second or 1 tick every 0.1 ms. This round number helps make further calculations easier.

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


## Part 2: UART Communication

### UART Protocol Explanation

#### What UART is and where it is used
UART (Universal Asynchronous Receiver-Transmitter) is a widely used hardware communication protocol for data exchange between devices. It is asynchronous i.e. it uses no shared clock signal and instead relies on pre-configured baud rates (speed) and specific data framing (bits) to communicate between two devices. It uses 2 wires — Transmit (TX) and Receive (RX) — for all methods of communication. UART is used in embedded systems for debugging, GPS modules, Bluetooth modules and communication with PC serial ports. 

#### Frame format
Frames are the way in which UART protocol works on. It is a data packet which consists of around 10-12 bits. When no data is transmitted, line is held high (1 state). The bits (in order) in a frame are as follows-
- Start bit : It is just a transition from high to low state.
- Data bits : These bits are what actaully transfer data. They are usually 8 in number and send data starting from the LSB.
- Parity bit : It is a bit put between data and stop bits and is used for error detection. Its value depends on if we are using even parity or odd parity.
- Stop bit(s) : After the data and parity bits, the stop bit indicates the end of user data. It's either a transition back to the idle state or remaining at the high state for an additional bit time.

#### Baud rate and its significance
Baud rate is the speed at which both the transmitter and receiver work. It is the number of bits transmitted per second. It makes the transmitter's and receiver's timing exact which could otherwise lead to corruption of data.

#### Asynchronous communication concept
Asynchronous communication is the exchange of data between transmitter (TX) and receiver (RX) without a shared clock signal. Thus, both deviced need to have a pre-configured speed (baud rate) and special bits that act as fixed intervals at which data is read.

### Implementation Details

#### Registers used for UART configuration
- RCC_AHB1ENR & RCC_APB1ENR : Enables GPIOA and USART2 clocks. 
- GPIOA_MODER : Used to set PA2 (TX) and PA3 (RX) to alterate function mode.
- GPIOA_AFRL : Used to indicate that PA2 and PA3 are controlled by TX and RX of USART2 respectively.
- USART2_BRR : Used to set baud rate divisor to achieve intended baud rate.
- USART2_CR1 : It enables TX, RX, USART.
- USART2_SR : This register has bits that are used to tell the status of the byte that is transmitted / received. Whether it's ready to be transmitted or read or completed, etc.
- USART2_DR : This is actually 2 registers having the same address that can work as a store to transmit bytes or store them until they are read by the microcontroller.

#### Baud rate calculation
The baud rate I'm aiming for is 115,200. The formula for the divisor is USARTDIV = clock frequency / (16 × BaudRate). Since UART is asynchronous so it oversamples the incoming line at 16 times the baud rate and picks the middle sample. This gives noise immunity. This is the reason for the 1/16 factor. Clock frequency is 16 MHz as that what APB1 bus gets. To get 115,200 baud rate, we get USARTDIV = 8.6085 which is 0x008B in hex.

#### Steps involved in transmitting data
1. When we press reset button on the board, all the pins that are to be configured once do so.
2.  The TXE in the status register flips to 1 which indicates that the byte is ready to be taken and transmitted.
3. The byte is then loaded to TX data register from where the shift register loads the byte in hex form and begins transmitting it to the RealTerm terminal bit by bit (LSB first). TXE remains at 0 whilst this happens.
4. After data has been transmitted, the TXE sets to 1 ready for another byte of data and our computer displays the sent bit.
