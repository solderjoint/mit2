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
#define PIN_BTN       GPIO_PIN_0 // switch button

#define PIN_LED3      GPIO_PIN_3 // prog mode indicator
#define PIN_LED2      GPIO_PIN_2 // read mode indicator
#define PIN_LED1      GPIO_PIN_1 // writing/reading indicator
#define PIN_LED_ON    (0x00)  // led on state
#define PIN_LED_OFF   (0xFF)  // led off state

#define PIN_ADDRESS1  GPIO_PIN_4 // module address selector
#define PIN_ADDRESS2  GPIO_PIN_5 // module address selector
#define PIN_ADDRESS3  GPIO_PIN_6 // module address selector

#define PIN_CODEID0   GPIO_PIN_0 // module codename hardwired
#define PIN_CODEID1   GPIO_PIN_1 // module codename hardwired
#define PIN_CODEID2   GPIO_PIN_5 // module codename hardwired
#define PIN_CODEID3   GPIO_PIN_6 // module codename hardwired
#define PIN_CODEID4   GPIO_PIN_7 // module codename hardwired

#define PIN_TRIGSET   GPIO_PIN_0 // trigger signal set
#define PIN_TRIGCLR   GPIO_PIN_1 // trigger signal reset
#define PIN_TRIGOUT   GPIO_PIN_3 // trigger signal output

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

void GpioButtonInit (void);
int32 GpioButtonGet (void);

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__GPIO_H
