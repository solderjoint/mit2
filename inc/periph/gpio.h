/*
 * Peripherials header for mcu on-board usart
 */

#ifndef __GPIO_H
#define __GPIO_H

#include "util/typedefs.h"
#include "util/util.h"

/* **************************************************** *
 *                  SYSTEM WIDE MACROS
 * **************************************************** */
enum gpioEnum {
	GPIO_BUTTON   = (1 << 0),  // switch button
	GPIO_LED3     = (1 << 3),  // prog mode indicator
	GPIO_LED2     = (1 << 2),  // read mode indicator
	GPIO_LED1     = (1 << 1),  // writing/reading indicator
	GPIO_LED_ON   = 1,         // led on state
	GPIO_LED_OFF  = 0,         // led off state
	GPIO_LED_XOR  = -1,        // led xor state

	GPIO_ADDRESS1 = (1 << 4),  // module address selector
	GPIO_ADDRESS2 = (1 << 5),  // module address selector
	GPIO_ADDRESS3 = (1 << 6),  // module address selector

	GPIO_MODULE0  = (1 << 0),  // module codename hardwired
	GPIO_MODULE1  = (1 << 1),  // module codename hardwired
	GPIO_MODULE2  = (1 << 5),  // module codename hardwired
	GPIO_MODULE3  = (1 << 6),  // module codename hardwired
	GPIO_MODULE4  = (1 << 7),  // module codename hardwired

	GPIO_TRIG_SET = (1 << 0),  // trigger signal set
	GPIO_TRIG_CLR = (1 << 1),  // trigger signal reset
	GPIO_TRIG_OUT = (1 << 3),  // trigger signal output
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void GpioTriggerInit (void);
void GpioTriggerSet (void);
void GpioTriggerReset (void);
uint8 GpioTriggerGet (void);

void GpioModuleAdressInit (void);
uint8 GpioModuleAdressGet (void);

void GpioModuleCodenameInit (void);
uint8 GpioModuleCodenameGet (void);

void GpioLedsInit (void);
void GpioLedsSet (uint8 pins, int8 val);
int8 GpioLedsGet (const uint8 pins);
uint8 GpioLedsPortGet (void);

void GpioButtonInit (void);
int32 GpioButtonGet (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__GPIO_H
