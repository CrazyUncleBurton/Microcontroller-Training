Digital Outputs Notes

M5Stack Tab5 uses ESP32-P4 processor.

GPIO are 40mA sink, 28mA source per pin, with 1,500mA cumulative GPIO output.

available GPIO:  2, 3, 4, 16, 45, 35, 47, 48, 51, 53, 54.  53 and 54 are PORTA so super accessible.

These pins are also configurable as inputs and outputs if you aren't using their alternate functionality:
Pin 	Bus	        Notes
GPIO18	SPI MOSI	external devices may drive bus
GPIO19	SPI MISO	input direction expected
GPIO5	SPI SCK	    clock line
GPIO38	UART0 RX	already input
GPIO37	UART0 TX	already output
GPIO7	PC_RX	    UART line
GPIO6	PC_TX	    UART line

Avoid using these:
GPIO21 / 20 / 34	RS485 interface
GPIO30 / 27 / 26 / 28 / 29	audio codec I2S
GPIO31, GPIO32:  I2C    They are open drain outputs.  They need a pullup to produce a voltage output.
GPIO36	camera clock



Voltage Level:
3.3V
Max 3.6V
Not 5V tolerant!



floating inputs
pullups
pulldowns
debounce



Adding Digital Input and output
MCP23017 Port Expander
Shift Register / chain of shift registers can be inputs or outputs
3.3V/5V input tolerant.
25mA/pin
