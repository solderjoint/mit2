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
