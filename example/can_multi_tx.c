
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

//*****************************************************************************
//
//! \addtogroup can_examples_list
//! <h1>Multiple CAN TX (multi_tx)</h1>
//!
//! This example shows how to set up the CAN to send multiple messages.  The
//! CAN peripheral is configured to send messages with 4 different CAN IDs.
//! Two of the messages (with different CAN IDs) are sent using a shared
//! message object.  This shows how to reuse a message object for multiple
//! messages.  The other two messages are sent using their own message objects.
//! All four messages are transmitted once per second.  The content of each
//! message is a test pattern.  A CAN interrupt handler is used to confirm
//! message transmission and count the number of messages that have been sent.
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - CAN0 peripheral
//! - GPIO Port B peripheral (for CAN0 pins)
//! - CAN0RX - PB4
//! - CAN0TX - PB5
//!
//! The following UART signals are configured only for displaying console
//! messages for this example.  These are not required for operation of CAN.
//! - GPIO port A peripheral (for UART0 pins)
//! - UART0RX - PA0
//! - UART0TX - PA1
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - INT_CAN0 - CANIntHandler
//
//*****************************************************************************

volatile uint32_t g_ui32IntCount = 0;

volatile uint32_t g_ui32Msg1Count = 0;
volatile uint32_t g_ui32Msg2Count = 0;
volatile uint32_t g_ui32Msg3Count = 0;

volatile bool g_bMsgObj3Sent = 0;
volatile bool g_bErrFlag = 0;

tCANMsgObject g_sCANMsgObject1;
tCANMsgObject g_sCANMsgObject2;
tCANMsgObject g_sCANMsgObject3;

uint8_t g_pui8Msg1[4] = { 0, 0, 0, 0 };
uint8_t g_pui8Msg2[5] = { 2, 2, 2, 2, 2 };
uint8_t g_pui8Msg3[6] = { 3, 3, 3, 3, 3, 3 };
uint8_t g_pui8Msg4[8] = { 4, 4, 4, 4, 5, 5, 5, 5 };

void
InitConsole(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTStdioConfig(0, 115200, 16000000);
}

void
PrintCANMessageInfo(tCANMsgObject *psCANMsg, uint32_t ui32MsgObj)
{
    unsigned int uIdx;
    UARTprintf("Sending msg: obj=%d ID=0x%04X msg=0x", ui32MsgObj,
               psCANMsg->ui32MsgID);
    for(uIdx = 0; uIdx < psCANMsg->ui32MsgLen; uIdx++)
        UARTprintf("%02X ", psCANMsg->pui8MsgData[uIdx]);
    UARTprintf("\n");
}

void
SimpleDelay(void)
{
    SysCtlDelay(16000000 / 3);
}

void
CANIntHandler(void)
{
    uint32_t ui32Status;
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    if(ui32Status == CAN_INT_INTID_STATUS) {
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        g_bErrFlag = 1;
    }

    else if(ui32Status == 1) {
        CANIntClear(CAN0_BASE, 1);
        g_ui32Msg1Count++;
        g_bErrFlag = 0;
    }
    else if(ui32Status == 2) {
        CANIntClear(CAN0_BASE, 2);
        g_ui32Msg2Count++;
        g_bErrFlag = 0;
    }
    else if(ui32Status == 3)
    {
        CANIntClear(CAN0_BASE, 3);
        g_ui32Msg3Count++;
        g_bMsgObj3Sent = 1;
        g_bErrFlag = 0;
    }
    else
    {
    }
}

int
main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    InitConsole();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    CANInit(CAN0_BASE);
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    IntEnable(INT_CAN0);
    CANEnable(CAN0_BASE);

    g_sCANMsgObject1.ui32MsgID = 0x1001;
    g_sCANMsgObject1.ui32MsgIDMask = 0;
    g_sCANMsgObject1.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    g_sCANMsgObject1.ui32MsgLen = sizeof(g_pui8Msg1);
    g_sCANMsgObject1.pui8MsgData = g_pui8Msg1;

    g_sCANMsgObject2.ui32MsgID = 0x2001;
    g_sCANMsgObject2.ui32MsgIDMask = 0;
    g_sCANMsgObject2.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    g_sCANMsgObject2.ui32MsgLen = sizeof(g_pui8Msg2);
    g_sCANMsgObject2.pui8MsgData = g_pui8Msg2;

    for(;;)
    {
        PrintCANMessageInfo(&g_sCANMsgObject1, 1);
        CANMessageSet(CAN0_BASE, 1, &g_sCANMsgObject1, MSG_OBJ_TYPE_TX);

        PrintCANMessageInfo(&g_sCANMsgObject2, 2);
        CANMessageSet(CAN0_BASE, 2, &g_sCANMsgObject2, MSG_OBJ_TYPE_TX);

        g_sCANMsgObject3.ui32MsgID = 0x3001;
        g_sCANMsgObject3.ui32MsgIDMask = 0;
        g_sCANMsgObject3.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
        g_sCANMsgObject3.ui32MsgLen = sizeof(g_pui8Msg3);
        g_sCANMsgObject3.pui8MsgData = g_pui8Msg3;

        g_bMsgObj3Sent = 0;

        PrintCANMessageInfo(&g_sCANMsgObject3, 3);
        CANMessageSet(CAN0_BASE, 3, &g_sCANMsgObject3, MSG_OBJ_TYPE_TX);

        while(!g_bMsgObj3Sent) { }

        g_sCANMsgObject3.ui32MsgID = 0x3002;
        g_sCANMsgObject3.ui32MsgIDMask = 0;
        g_sCANMsgObject3.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
        g_sCANMsgObject3.ui32MsgLen = sizeof(g_pui8Msg4);
        g_sCANMsgObject3.pui8MsgData = g_pui8Msg4;

        PrintCANMessageInfo(&g_sCANMsgObject3, 3);
        CANMessageSet(CAN0_BASE, 3, &g_sCANMsgObject3, MSG_OBJ_TYPE_TX);

        SimpleDelay();

        (*(uint32_t *)g_pui8Msg1)++;
        (*(uint32_t *)g_pui8Msg2)++;
        (*(uint32_t *)g_pui8Msg3)++;
        (*(uint32_t *)&g_pui8Msg4[0])++;
        (*(uint32_t *)&g_pui8Msg4[4])--;
    }

    return(0);
}
