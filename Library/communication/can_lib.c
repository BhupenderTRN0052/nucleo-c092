#include "can_lib.h"

extern Queue TxQueue;
extern Queue RxQueue;

/* This function is used to enable CAN hardware filter and to start the CAN module*/
void FDCAN_Setup()
{
	// 1. initialize ConfigGlobalFilter
	if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK)
	{
		Error_Handler();
	}

	// 2. create filter list for IDs that you want to recieve in fifo
	CAN_Filter_IDList(SOLAR_CORE_RX_TEST_ID, S, FBANK0, FIFO0_CAN1);

	// 3. start CAN
	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
	{
		Error_Handler();
	}

	// 4. activate notification for new msg in fifo
	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK)
	{
		Error_Handler();
	}
}

/* Transmit a CAN message. */
void Transmit_on_CAN(uint32_t U32_transmitCANid, TypeofCANID U8_idType, uint8_t *U8_dataarr, uint8_t U8_DLC)
{
	CAN_Message msg;
	if (U8_idType == E)
	{
		msg.TxHeader.IdType = FDCAN_EXTENDED_ID;
	}
	else if (U8_idType == S)
	{
		msg.TxHeader.IdType = FDCAN_STANDARD_ID;
	}

	msg.TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	msg.TxHeader.Identifier = U32_transmitCANid;
	msg.TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	msg.TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	msg.TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	msg.TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	msg.TxHeader.MessageMarker = 0;
	msg.TxHeader.DataLength = U8_DLC;

	memcpy(msg.Data, U8_dataarr, U8_DLC);
	enqueue(&TxQueue, &msg);
}

/* Configure Hardware CAN filter */
void CAN_Filter_IDList(uint32_t U32_receiveCANid1, TypeofCANID U8_idType, FilterBanks_CAN1 U8_filterbank, FIFOs_CAN1 U8_fifo)
{
	FDCAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterType  = FDCAN_FILTER_DUAL;
	sFilterConfig.FilterIndex = U8_filterbank;

	if (U8_idType == E)
	{
		sFilterConfig.IdType    = FDCAN_EXTENDED_ID;
		sFilterConfig.FilterID1 = (U32_receiveCANid1 & 0x1FFFE000) >> 13;
		sFilterConfig.FilterID2 = ((U32_receiveCANid1 & 0x00001FFF) << 3) | 0x04;
	}
	else if (U8_idType == S)
	{
		sFilterConfig.IdType    = FDCAN_STANDARD_ID;
		sFilterConfig.FilterID1 = U32_receiveCANid1;
		sFilterConfig.FilterID2 = 0x556;
	}
	if (U8_fifo == FIFO0_CAN1)
	{
		sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	}
	else if (U8_fifo == FIFO1_CAN1)
	{
		sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	}
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK)
    {
        Error_Handler();
    }
}

/* Transmit the Tx Queue*/
void Transmit_TxQueue(void)
{
	CAN_Message msg;

	if (!isEmpty(&TxQueue))
	{
		if (dequeue(&TxQueue, &msg) == 0)
		{
			if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &msg.TxHeader, msg.Data) != HAL_OK)
			{
				enqueue(&TxQueue, &msg);
			}
		}
	}
}

/* Recieve Queue */
void Process_RxQueue(void)
{
	CAN_Message msg;
	if (!isEmpty(&RxQueue))
	{
		if (dequeue(&RxQueue, &msg) == 0)
		{
			// CAN IDs will be processed here.
			if(msg.RxHeader.Identifier == SOLAR_CORE_RX_TEST_ID)
			{
				//process CAN msg here
			}
			else
			{
				// Process other messages
			}
		}
	}
}

/* Callback function for FDCAN1 FIFO0 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if (hfdcan == &hfdcan1)
	{
		FDCAN_RxHeaderTypeDef RxHeader;
		CAN_Message msg;
		uint8_t RxData[8];
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData);

		msg.RxHeader = RxHeader;
		memcpy(msg.Data, RxData, sizeof(RxData));
		enqueue(&RxQueue, &msg);
	}
}

/* send UUID over CAN  */
void send_UUID(uint32_t *uid)
{
	uid[0] = HAL_GetUIDw0();
	uid[1] = HAL_GetUIDw1();
	uid[2] = HAL_GetUIDw2();
    uint8_t uuid_bytes[12];  
    uint8_t TxData[8];

    uuid_bytes[0] = (uid[0] >> 24) & 0xFF;    
    uuid_bytes[1] = (uid[0] >> 16) & 0xFF;   
    uuid_bytes[2] = (uid[0] >> 8) & 0xFF;
    uuid_bytes[3] = (uid[0] >> 0) & 0xFF;

    uuid_bytes[4] = (uid[1] >> 24) & 0xFF; 
    uuid_bytes[5] = (uid[1] >> 16) & 0xFF; 
    uuid_bytes[6] = (uid[1] >> 8) & 0xFF;
    uuid_bytes[7] = (uid[1] >> 0) & 0xFF;

    uuid_bytes[8] = (uid[2] >> 24) & 0xFF; 
    uuid_bytes[9] = (uid[2] >> 16) & 0xFF; 
    uuid_bytes[10] = (uid[2] >> 8) & 0xFF;
    uuid_bytes[11] = (uid[2] >> 0) & 0xFF;

    TxData[0] = SOLAR_CORE_ECU_ID;          
    TxData[1] = SOLAR_CORE_HW_VERSION_MAJOR; 	// HW MAJAR
    TxData[2] = SOLAR_CORE_HW_VERSION_MINOR; 	// HW MINOR
    TxData[3] = SOLAR_CORE_HW_VERSION_PATCH; 	// HW PATCH
    TxData[4] = SOLAR_CORE_SW_VERSION_MAJOR; 	// SW MAJOR
    TxData[5] = SOLAR_CORE_SW_VERSION_MINOR;	// SW MINOR
    TxData[6] = SOLAR_CORE_SW_VERSION_PATCH;	// SW PATCH
    TxData[7] = uuid_bytes[0];
	Transmit_on_CAN(SOLAR_CORE_TX_UUID0, S, TxData, 8);

    TxData[0] = SOLAR_CORE_ECU_ID;          
    TxData[1] = 0x00;
    TxData[2] = 0x00;
    TxData[3] = 0x00;
    TxData[4] = uuid_bytes[1];
    TxData[5] = uuid_bytes[2];
    TxData[6] = uuid_bytes[3];
    TxData[7] = uuid_bytes[4];
	Transmit_on_CAN(SOLAR_CORE_TX_UUID1, S, TxData, 8);

    TxData[0] = SOLAR_CORE_ECU_ID;          
    TxData[1] = uuid_bytes[5];
    TxData[2] = uuid_bytes[6];
    TxData[3] = uuid_bytes[7];
    TxData[4] = uuid_bytes[8];
    TxData[5] = uuid_bytes[9];
    TxData[6] = uuid_bytes[10];
    TxData[7] = uuid_bytes[11];
	Transmit_on_CAN(SOLAR_CORE_TX_UUID2, S, TxData, 8);
}


/* Device SW version*/
void send_SW_Version(void)
{
	uint8_t TxData[8];
	TxData[0] = SOLAR_CORE_ECU_ID;          	
	TxData[1] = SOLAR_CORE_SW_VERSION_MAJOR; 	
	TxData[2] = SOLAR_CORE_SW_VERSION_MINOR; 	
	TxData[3] = SOLAR_CORE_SW_VERSION_PATCH; 	
	TxData[4] = 0x00; 	
	TxData[5] = 0x00;	
	TxData[6] = 0x00;	
	TxData[7] = 0x00;
	Transmit_on_CAN(SOLAR_CORE_TX_SW_VER, S, TxData, 8);
	Transmit_TxQueue();
}


/* Device HW version*/
void send_HW_Version(void)
{
	uint8_t TxData[8];
	TxData[0] = SOLAR_CORE_ECU_ID;          	
	TxData[1] = SOLAR_CORE_HW_VERSION_MAJOR; 	
	TxData[2] = SOLAR_CORE_HW_VERSION_MINOR; 	
	TxData[3] = SOLAR_CORE_HW_VERSION_PATCH; 	
	TxData[4] = 0x00; 	
	TxData[5] = 0x00;	
	TxData[6] = 0x00;	
	TxData[7] = 0x00;
	Transmit_on_CAN(SOLAR_CORE_TX_HW_VER, S, TxData, 8);
	Transmit_TxQueue();
}