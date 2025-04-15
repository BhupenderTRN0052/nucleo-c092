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
	CAN_Filter_IDList(FDCANID_TEST_ID, S, FBANK0, FIFO0_CAN1);

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
			if(msg.RxHeader.Identifier == FDCANID_TEST_ID)
			{
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
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