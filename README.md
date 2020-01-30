# Line Control System project
Calculation module firmware project (designed to use with TM4C1231H6 mcu and MIT2 module).
rev 1C since august 2019

The goal of this project is to complete following routines:

 * check pin designated as a line endpoint signal catcher
 * check line voltage, catch overflow and underflow
 * perform fourier transform to catch signals
 * complete specified can protocol transactions
 * perform startup diagnostics to check hardware errors
 * implement a bootloader with firmware update ability

## todos
* create a list of variables ready for a modbus table
* perform startup diagnostics to check hardware errors
 * implement a bootloader with firmware update ability
 

# project folder map
## comm
Communication protocol handlers.

* smolin is a specific extended protocol built on top of a can protocol; it uses 8 bytes long can message to receive and transmit data; 29-bits extended can id is used here to act as a header for communication
* data/longword is a raw forth messaging standard (not implemented atm)
* data/modbus is a handler for 'modbus' specification of smolin protocol that allows greater flexibility when dealing with foreign devices that do not support longword format
* data/quick handles fast data communication standard which is set by a single positive bit in 'fastdata' field; fastdata allows the receiver to accepts 8 bytes of 'quick' data and to send back the same amount of data


|          |         |        |        |         |          |         |          |
|----------|---------|--------|--------|---------|----------|---------|----------|
|  MODBUS  |  MODBUS |  DEST  | SOURCE | TEXTEND | FASTDATA | REQUEST |   TEXT   |
| FUNCTION | REQUEST |   ID   |   ID   |   BIT   |   BIT    |   BIT   | POSITION |
|  3 bits  |  1 bit  | 8 bits | 8 bits |  1 bit  |  1 bit   |  1 bit  |  5 bits  |

## logic
Main program logic.

* crash handles the most important logic of the program: line and relay status checking, voltage checking, crash situations
* config is used as a start-up initialization routine for peripherals and configurable variables
* data/db is a simple (no self-healing ability) single key single value hash table with access by unique id, both keys and values are stored in eeprom; single entry takes just 8 bytes of data (key is a int32, value can be represented as both int32 and float)
* math/domain is used to handle both pre-processing of input voltage/time buffer and post-processing of resulting power/frequency buffer
* math/fft1024 is a Fourier procedure that accepts voltage/time buffer of 1024 complex float32 values (each complex value has real and imaginary part) and outputs the resulting power/frequency array of 1024 complex float32 values; this part alone takes 16Kbytes of memory
* math/linecheck is a wrapper that is used to check current line status, relay state and voltage
* math/sampling is a wrapper for Fourier transform that controls the correct timing of voltage sampling and it also controls oversampling
 
## periph
Drivers used to initialize and handle work of various peripherals.

* can (controller area network) is set to startup at 100 kHz with extended id and 8 bytes message length (baudrate and device id can be changed)
* gpio handles general pins, leds; hard-coded pins (and few replaceable pls-2 pinouts) set the module id in the system
* rom is a wrapper for long retention memory and is used to store configuration values
* spi is configured to to 800K 16bits and is used to request data from external analog to digital converter
* timer is configured for usage as a generic clock timer (ticks at 1MHz), semaphore handler (ticks every 1 msec), and sampling timer (used exclusively for Fourier transform)
* usart is configured to 115K2 8N1 (baudrate can be changed) and is used as generic console output

## system
System files that are used by the selected microcontroller.

* diagnostic is a wrapper for system diagnostic routines (not implemented atm)
* startup is a primary starting procedure for arm-based microcontrollers (also requires project.ld file which configures memory locations)
* syscalls is a wrapper for the functions required by default libc, yet that are missing in arm-gcc-none-eabi library

## util
Generic utilities for the project.

* crc is a general library for calculating crc16/32/64
* debug is a testing library (not properly implemented)
* print is a generic printf library for domain-specific console usage
* util is a generic set of macros and utilities for time handling

## vars
Is a set of separate wrappers for global (configurable) variables.
