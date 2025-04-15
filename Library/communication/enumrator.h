#ifndef __ENUMERATOR_H
#define __ENUMERATOR_H


/* Fault code*/
typedef enum {
	FAULT_BUS_VOLTAGE	 = 1,
	FAULT_CURRENT_SENSE	 = 2,
	FAULT_PCB_TEMP		 = 3,
	FAULT_HEATER_TEMP	 = 4,
}FAULT_STATUS;

/*CAN Enumerators*/
typedef enum {
	E 	= 0,
	S	= 1,
}TypeofCANID;

typedef enum{
	FIFO0_CAN1 = 1,
	FIFO1_CAN1 = 2,
}FIFOs_CAN1;

typedef enum {
	FBANK0		= 0,
	FBANK1		= 1,
	FBANK2		= 2,
	FBANK3		= 3,
	FBANK4		= 4,
	FBANK5		= 5,
	FBANK6		= 6,
	FBANK7		= 7,
	FBANK8		= 8,
	FBANK9		= 9,
	FBANK10		= 10,
	FBANK11		= 11,
	FBANK12		= 12,
	FBANK13		= 13,
}FilterBanks_CAN1;


#endif
