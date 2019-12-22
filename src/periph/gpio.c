
#include "periph/gpio.h"

#include <stdbool.h>
#include <stdint.h>

#define  PART_TM4C1231H6PZ
#include <tivaware/inc/hw_gpio.h>
#include <tivaware/inc/hw_memmap.h>
#include <tivaware/inc/hw_types.h>

#include <tivaware/driverlib/gpio.h>
#include <tivaware/driverlib/interrupt.h>
#include <tivaware/driverlib/pin_map.h>
#include <tivaware/driverlib/sysctl.h>

/* **************************************************** *
 *             D-TRIGGER PINS MANIPULATION
 * **************************************************** */
void GpioTriggerInit (void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, PIN_TRIGSET | PIN_TRIGCLR);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, PIN_TRIGOUT);
	SysCtlDelay(100);

	GPIOPinWrite(GPIO_PORTB_BASE, PIN_TRIGCLR, 0xFF);
	GPIOPinWrite(GPIO_PORTB_BASE, PIN_TRIGSET, 0x00);
}

void GpioTriggerSet (void) {
	// turn off opto-relay / break the line
	GPIOPinWrite(GPIO_PORTB_BASE, PIN_TRIGSET, 0x00);
	SysCtlDelay(1000);
	GPIOPinWrite(GPIO_PORTB_BASE, PIN_TRIGSET, 0xFF);
}

void GpioTriggerReset (void) {
	// reset trigger state
	GPIOPinWrite(GPIO_PORTB_BASE, PIN_TRIGCLR, 0x00);
	SysCtlDelay(1000);
	GPIOPinWrite(GPIO_PORTB_BASE, PIN_TRIGCLR, 0xFF);
}

uint8 GpioTriggerGet (void) {
	const uint8 res = GPIOPinRead(GPIO_PORTB_BASE, PIN_TRIGOUT) && 0xFF;
	return res;
}

/* **************************************************** *
 *         PROGRAMMABLE MODULE ADDRESS SECTION
 * **************************************************** */
void GpioModuleAdressInit (void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, \
			PIN_ADDRESS1 | PIN_ADDRESS2 | PIN_ADDRESS3);
	SysCtlDelay(100);
}

uint8 GpioModuleAdressGet (void) {
	const uint8 read = GPIOPinRead(GPIO_PORTE_BASE, \
			PIN_ADDRESS1 | PIN_ADDRESS2 | PIN_ADDRESS3);
	const uint8 mask = (~(read >> 4)) & 0x07; // 0111b mask
	return mask;
}

/* **************************************************** *
 *       HARD-WIRED MODULE CODENAME PINS SECTION
 * **************************************************** */
void GpioModuleCodenameInit (void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

	// pin F0 is hardcoded to another function, unlock it
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x01;
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0,
		GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, \
			PIN_CODEID1 | PIN_CODEID2 | PIN_CODEID3 | PIN_CODEID4);
	SysCtlDelay(100);
}

uint8 GpioModuleCodenameGet (void) {
	const uint8 high = GPIOPinRead(GPIO_PORTF_BASE, \
			PIN_CODEID2 | PIN_CODEID3 | PIN_CODEID4);
	const uint8 low = GPIOPinRead(GPIO_PORTF_BASE, PIN_CODEID0 | PIN_CODEID1);
	return (high >> 3) + low;
}

/* **************************************************** *
 *                  LEDS PINS SECTION
 * **************************************************** */
void GpioLedsInit (void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK));

	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, \
			PIN_LED1 | PIN_LED2 | PIN_LED3);
	SysCtlDelay(1000);
	GPIOPinWrite(GPIO_PORTK_BASE, \
			PIN_LED1 | PIN_LED2 | PIN_LED3, PIN_LED_OFF);
	SysCtlDelay(100);
}

void GpioLedsSet (uint8 pins, int8 val) {
	uint8 mask;
	if (pins == 1) mask = PIN_LED1;
	else if (pins == 2) mask = PIN_LED2;
	else if (pins == 3) mask = PIN_LED3;

	if (val > 0) GPIOPinWrite(GPIO_PORTK_BASE, mask, PIN_LED_ON);
	else if (val == 0) GPIOPinWrite(GPIO_PORTK_BASE, mask, PIN_LED_OFF);
	else {
		const uint8 new = ~(GPIOPinRead(GPIO_PORTK_BASE, mask));
		GPIOPinWrite(GPIO_PORTK_BASE, mask, new);
	}
}

/* **************************************************** *
 *                  BUTTON PIN SECTION
 * **************************************************** */
void GpioButtonInit (void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK));

	GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, PIN_BTN);
	SysCtlDelay(100);
}

int32 GpioButtonGet (void) {
	const int32 res = GPIOPinRead(GPIO_PORTK_BASE, PIN_BTN);
	return res;
}
