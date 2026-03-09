# Digital Input and Output
by Bryan A. "CrazyUncleBurton" Thompson  
<bryan@batee.com>  
Last Updated 3/9/2026  

## Goals

digital read
digital write
toggle
floating inputs
pullups
pulldowns
debounce

----------

# Lesson 4 — Digital Inputs and Outputs

## Objective

- Learn how to read buttons and switches
- Learn how to turn a GPIO pin on and off

## Concepts

- GPIO input
- pull-up resistors
- debouncing

## Hardware

- push button
- resistor

## Circuit

![Button Circuit](assets/button-circuit.png)

## Examples

- button-polling
- button-interrupt

--------------------------------

M5Stack Tab5 uses ESP32-P4 processor.
GPIO Voltage Level: 3.3V typ, Max 3.6V, not 5V tolerant!

GPIO are 40mA sink, 28mA source per pin, with 1,500mA cumulative GPIO output.

Available GPIO:  2, 3, 4, 16, 45, 35, 47, 48, 51, 53, 54.  53 and 54 are PORTA so super accessible.

Pins Available if you're not using their alternate functionality:
Pin 	Bus	        Notes
GPIO18	SPI MOSI	external devices may drive bus
GPIO19	SPI MISO	input direction expected
GPIO5	SPI SCK	    clock line
GPIO38	UART0 RX	already input
GPIO37	UART0 TX	already output
GPIO7	PC_RX	    UART line
GPIO6	PC_TX	    UART line

Avoid using these pins:
GPIO21 / 20 / 34	RS485 interface
GPIO30 / 27 / 26 / 28 / 29	audio codec I2S
GPIO31, GPIO32:  I2C    They are open drain outputs.  They need a pullup to produce a voltage output.
GPIO36	camera clock

----------

Adding Digital Input and output
MCP23017 Port Expander
Shift Register / chain of shift registers can be inputs or outputs
3.3V/5V input tolerant.
25mA/pin
