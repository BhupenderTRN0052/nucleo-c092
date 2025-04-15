#ifndef CAN_LIB_H
#define CAN_LIB_H

#include "main.h"
#include "enumrator.h"
#include "queue.h"
#include <string.h>
#include "fdcan.h"

#define FDCANID_TEST_ID  0X555 

void FDCAN_Setup(void);
void Transmit_on_CAN(uint32_t U32_transmitCANid, TypeofCANID U8_idType, uint8_t* U8_dataarr, uint8_t U8_DLC);
void CAN_Filter_IDList(uint32_t U32_receiveCANid1, TypeofCANID U8_idType, FilterBanks_CAN1 U8_filterbank, FIFOs_CAN1 U8_fifo);
void Transmit_TxQueue(void);
void Process_RxQueue(void);




#endif // CAN_LIB_H