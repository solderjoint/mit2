
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

/* **************************************************** */
enum internalGpioEnum {
	LED_STATE_ON   = (0x00),    // led on state
	LED_STATE_OFF  = (0xFF),    // led off state
};

/* **************************************************** *
 *             D-TRIGGER PINS MANIPULATION
 * **************************************************** */
void GpioTriggerInit (void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_TRIG_SET | GPIO_TRIG_CLR);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_TRIG_OUT);
	SysCtlDelay(100);

	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_TRIG_CLR, 0xFF);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_TRIG_SET, 0x00);
}

void GpioTriggerSet (void) {
	// turn off opto-relay / break the line
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_TRIG_SET, 0x00);
	SysCtlDelay(1000);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_TRIG_SET, 0xFF);
}

void GpioTriggerReset (void) {
	// reset trigger state
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_TRIG_CLR, 0x00);
	SysCtlDelay(1000);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_TRIG_CLR, 0xFF);
}

uint8 GpioTriggerGet (void) {
	const uint8 res = GPIOPinRead(GPIO_PORTB_BASE, GPIO_TRIG_OUT) && 0xFF;
	return res;
}

/* **************************************************** *
 *         PROGRAMMABLE MODULE ADDRESS SECTION
 * **************************************************** */
void GpioModuleAdressInit (void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, \
			GPIO_ADDRESS1 | GPIO_ADDRESS2 | GPIO_ADDRESS3);
	SysCtlDelay(100);
}

uint8 GpioModuleAdressGet (void) {
	const uint8 read = GPIOPinRead(GPIO_PORTE_BASE, \
			GPIO_ADDRESS1 | GPIO_ADDRESS2 | GPIO_ADDRESS3);
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
			GPIO_MODULE1 | GPIO_MODULE2 | GPIO_MODULE3 | GPIO_MODULE4);
	SysCtlDelay(100);
}

uint8 GpioModuleCodenameGet (void) {
	const uint8 high = GPIOPinRead(GPIO_PORTF_BASE, \
			GPIO_MODULE2 | GPIO_MODULE3 | GPIO_MODULE4);
	const uint8 low = GPIOPinRead(GPIO_PORTF_BASE, GPIO_MODULE0 | GPIO_MODULE1);
	return (high >> 3) + low;
}

/* **************************************************** *
 *                  LEDS PINS SECTION
 * **************************************************** */
void GpioLedsInit (void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK));

	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, \
			GPIO_LED1 | GPIO_LED2 | GPIO_LED3);
	SysCtlDelay(1000);
	GPIOPinWrite(GPIO_PORTK_BASE, \
			GPIO_LED1 | GPIO_LED2 | GPIO_LED3, LED_STATE_OFF);
	SysCtlDelay(100);
}

void GpioLedsSet (uint8 mask, int8 val) {
	switch (val) {
		case GPIO_LED_ON:
			GPIOPinWrite(GPIO_PORTK_BASE, mask, LED_STATE_ON);
			break;

		case GPIO_LED_XOR:
			GPIOPinWrite(GPIO_PORTK_BASE, mask, \
					~(GPIOPinRead(GPIO_PORTK_BASE, mask)));
			break;

		default:
			GPIOPinWrite(GPIO_PORTK_BASE, mask, LED_STATE_OFF);
			break;
	}
}

int8 GpioLedsGet (const uint8 pins) {
	return (GPIOPinRead(GPIO_PORTK_BASE, pins) > 0);
}

uint8 GpioLedsPortGet (void) {
	return (uint8) GPIOPinRead(GPIO_PORTK_BASE, 0xFF);
}

/* **************************************************** *
 *                  BUTTON PIN SECTION
 * **************************************************** */
void GpioButtonInit (void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK));

	GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_BUTTON);
	SysCtlDelay(100);
}

int32 GpioButtonGet (void) {
	const int32 res = GPIOPinRead(GPIO_PORTK_BASE, GPIO_BUTTON);
	return res;
}
