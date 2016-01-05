# Arduino motor controller
This code is meant to run on a standalone BLDC motor controller, based on the ATmega328p. 
The code is interrupt based, and takes step and direction for each motor. A step will occur on a rising edge.

## Connection
#### Motor 1
Step: PC1 (Analog pin 1) <br/>
Direction: PD2 (Digital pin 2)

#### Motor 2
Step: PC2 (Analog pin 2) <br/>
Direction: PC3 (Analog pin 3)


