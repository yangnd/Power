#ifndef __TASK_READBAT_H
#define __TASK_READBAT_H
#include "sys.h"

#define MODBUS_ACK_OK ( 0 )
#define MODBUS_ACK_NOTOK ( 1 )
#define MODBUS_ACK_CRC_ERR ( 2 )
#define MODBUS_ACK_FRAME_ERR ( 3 )
#define MODBUS_ACK_TIMEOUT ( 4 )
#define MODBUS_ACK_LOOPERR ( 5 )
#define MODBUS_ACK_SENDERR ( 6 )

#define BAT_COMM_ERR 0
#define BAT_CHG_START 1
#define BAT_CHG_COMPTETE 2

typedef struct {
    uint8_t  id;
    uint8_t  cmd;
    uint8_t  addrH;
    uint8_t  addrL;
    uint8_t  lenH;
    uint8_t  lenL;
    uint16_t crc;
} modbusCmd_t;

void Modbus_Init( void );
void vReadBatTask( void* param );
u8   getBatStatus( void );

#endif
