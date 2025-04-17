#ifndef CAN_LIB_H
#define CAN_LIB_H

#include "main.h"
#include "enumrator.h"
#include "queue.h"
#include <string.h>
#include "fdcan.h"

#define SOLAR_CORE_ECU_ID                        0x07

#define SOLAR_CORE_SW_VERSION_MAJOR     (uint8_t)0x00
#define SOLAR_CORE_SW_VERSION_MINOR     (uint8_t)0x00
#define SOLAR_CORE_SW_VERSION_PATCH     (uint8_t)0x09

#define SOLAR_CORE_HW_VERSION_MAJOR     (uint8_t)0x01
#define SOLAR_CORE_HW_VERSION_MINOR     (uint8_t)0x00
#define SOLAR_CORE_HW_VERSION_PATCH     (uint8_t)0x00

#define SOLAR_CORE_RX_TEST_ID                 0X555 
#define SOLAR_CORE_TX_SW_VER                  0X7A1
#define SOLAR_CORE_TX_HW_VER                  0X7A2
#define SOLAR_CORE_TX_UUID0                   0X6F0
#define SOLAR_CORE_TX_UUID1                   0X6F1
#define SOLAR_CORE_TX_UUID2                   0X6F2

void FDCAN_Setup(void);
void Transmit_on_CAN(uint32_t U32_transmitCANid, TypeofCANID U8_idType, uint8_t* U8_dataarr, uint8_t U8_DLC);
void CAN_Filter_IDList(uint32_t U32_receiveCANid1, TypeofCANID U8_idType, FilterBanks_CAN1 U8_filterbank, FIFOs_CAN1 U8_fifo);
void Transmit_TxQueue(void);
void Process_RxQueue(void);
void send_UUID(uint32_t *uid);
void send_SW_Version(void);
void send_HW_Version(void);




#endif // CAN_LIB_H