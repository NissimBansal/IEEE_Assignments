## Part 1: Analog-to-Digital Converter

### What is an ADC?
ADC or Analog-to-Digital Converter an electronic component or external device that converts a continuous physical analog signal such as temperature, light, sound - or in our case voltage - into discrete numbers that can be understood by microprocessors and such.

### What is resolution in ADC?
Resolution defines the number of discrete values that are accpeted by a microprocessor. It is given in terms of number of bits (example: 12-bit/16-bit). The total number discrete values are then given by raising 2 to the power of the number of bits defined above. A higher resolution means a smaller detectable voltage change i.e. more accuracy.

### ADC used in STM32 and its working
Most STM32 microcontrollers use Successive Approximation Register or SAR ADCs. It is called so because its mechanism involves successively approximating input voltage. Here's how it works exactly-
1. The input switch is closed charging an internal capacitor to that voltage level. This is called V_IN.
2. The SARs in STM sets the SAR's MSB to 1 which the internal DAC converts to an analog voltage level.
3. A comparator checks this voltage from the capacitor voltage and updates SAR.
4. The process moves to the next bit. SAR sets it to 1, the DAC produces a new voltage based on the updated register and the comparator checks the result again. This continues down to all the bits and final result is stored in ADC data register.

### List of all registers used
