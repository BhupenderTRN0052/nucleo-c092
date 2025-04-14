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

	// 2. create filter list for IDs that you want to recieve in fifo+
	// CAN_Filter_IDList(FDCANID_FRM_UP_RX_3FE, S, FBANK15, FIFO1_CAN2);

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


/* Configure Hardware CAN filter */
void CAN_Filter_IDList(uint32_t U32_receiveCANid1, TypeofCANID U8_idType, FilterBanks_CAN2 U8_filterbank, FIFOs_CAN2 U8_fifo)
{
	FDCAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterType  = FDCAN_FILTER_MASK;
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
		sFilterConfig.FilterID2 = 0x7FF;
	}
	if (U8_fifo == FIFO0_CAN2)
	{
		sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	}
	else if (U8_fifo == FIFO1_CAN2)
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

/* Transmit a CAN message. */
// void Transmit_on_CAN(uint32_t U32_transmitCANid, TypeofCANID U8_idType, uint8_t *U8_dataarr, uint8_t U8_DLC)
// {
// 	CAN_Message msg;
// 	if (U8_idType == E)
// 	{
// 		msg.TxHeader.IdType = FDCAN_EXTENDED_ID;
// 	}
// 	else if (U8_idType == S)
// 	{
// 		msg.TxHeader.IdType = FDCAN_STANDARD_ID;
// 	}

// 	msg.TxHeader.TxFrameType         = FDCAN_DATA_FRAME;
// 	msg.TxHeader.Identifier          = U32_transmitCANid;
// 	msg.TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
// 	msg.TxHeader.BitRateSwitch       = FDCAN_BRS_OFF;
// 	msg.TxHeader.FDFormat            = FDCAN_CLASSIC_CAN;
// 	msg.TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
// 	msg.TxHeader.MessageMarker       = 0;
// 	msg.TxHeader.DataLength 		 = U8_DLC;

// 	memcpy(msg.Data, U8_dataarr, U8_DLC);

// 	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &msg.TxHeader, msg.Data) != HAL_OK)
// 	{
// 		Error_Handler();
// 	}

// 	// memcpy(msg.Data, U8_dataarr, U8_DLC);
// 	// enqueue(&TxQueue, &msg);
// }

void Transmit_on_CAN(uint32_t U32_transmitCANid, TypeofCANID U8_idType, uint8_t *U8_dataarr, uint8_t U8_DLC) 
{
	FDCAN_TxHeaderTypeDef	TxMsg;
    
	if(U8_idType == E)      TxMsg.IdType = FDCAN_EXTENDED_ID;
	else if(U8_idType == S) TxMsg.IdType = FDCAN_STANDARD_ID;

	TxMsg.TxFrameType           = FDCAN_DATA_FRAME;
	TxMsg.Identifier            = U32_transmitCANid;
    TxMsg.ErrorStateIndicator   = FDCAN_ESI_ACTIVE;
    TxMsg.BitRateSwitch         = FDCAN_BRS_OFF;
    TxMsg.FDFormat              = FDCAN_CLASSIC_CAN;
    TxMsg.TxEventFifoControl    = FDCAN_NO_TX_EVENTS;
    TxMsg.MessageMarker         = 0;
	
	switch(U8_DLC) { 
		case 0:  TxMsg.DataLength  = FDCAN_DLC_BYTES_0; break;  /*!< 0 bytes data field  */
		case 1:  TxMsg.DataLength  = FDCAN_DLC_BYTES_1; break;  /*!< 1 bytes data field  */
		case 2:  TxMsg.DataLength  = FDCAN_DLC_BYTES_2; break;  /*!< 2 bytes data field  */
		case 3:  TxMsg.DataLength  = FDCAN_DLC_BYTES_3; break;  /*!< 3 bytes data field  */
		case 4:  TxMsg.DataLength  = FDCAN_DLC_BYTES_4; break;  /*!< 4 bytes data field  */
		case 5:  TxMsg.DataLength  = FDCAN_DLC_BYTES_5; break;  /*!< 5 bytes data field  */
		case 6:  TxMsg.DataLength  = FDCAN_DLC_BYTES_6; break;  /*!< 6 bytes data field  */
		case 7:  TxMsg.DataLength  = FDCAN_DLC_BYTES_7; break;  /*!< 7 bytes data field  */
		case 8:  TxMsg.DataLength  = FDCAN_DLC_BYTES_8; break;  /*!< 8 bytes data field  */
		default: TxMsg.DataLength  = FDCAN_DLC_BYTES_0; break;  /*!< 0 bytes data field  */
	}

    
	if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxMsg, U8_dataarr) != HAL_OK) 
	  __HAL_FDCAN_CLEAR_FLAG(&hfdcan1, FDCAN_TXBC_TFQM);

}


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