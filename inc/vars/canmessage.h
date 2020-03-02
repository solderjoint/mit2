#ifndef __VAR_CANMSG_H
#define __VAR_CANMSG_H

#include "util/typedefs.h"

/* **************************************************** *
 *                  HEADER SPECIFIC MACROS
 * **************************************************** */
enum _var_canEnum {
	CAN_RECV_OBJ_ID  = 1,   // default object mesage id
	CAN_SEND_OBJ_ID  = 4,   // default object mesage id
	CAN_MSGBUF_LEN   = 8,   // standard maximum message length
};

/* **************************************************** *
 *                 FUNCTION PROTOTYPES
 * **************************************************** */
void CanMessageInit (void);

void CanMessageReceive(void);
uint8* CanMessageReceiverBufferGet (void);
int32 CanMessageReceiverSizeGet (void);
int32 CanMessageReceiverIdGet (void);

void CanMessageSend (void);
void CanMessageSenderBufferSet(const uint8 buffer[CAN_MSGBUF_LEN]);
void CanMessageSenderIdSet (const int32 id);

/* **************************************************** */
char CanMessageReceiverFlagGet (void);
void CanMessageReceiverFlagSet (void);
void CanMessageReceiverFlagClear (void);

char CanMessageSenderFlagGet (void);
void CanMessageSenderFlagSet (void);
void CanMessageSenderFlagClear (void) ;

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__VAR_CANMSG_H
