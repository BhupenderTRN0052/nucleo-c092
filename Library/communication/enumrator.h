#ifndef __ENUMERATOR_H
#define __ENUMERATOR_H


/*CAN Enumerators*/
typedef enum {
	E 	= 0,
	S	= 1,
}TypeofCANID;

typedef enum{
	FIFO0_CAN1 = 0,
	FIFO1_CAN1 = 1,
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

typedef enum{
	FBANK14		= 14,
	FBANK15		= 15,
	FBANK16		= 16,
	FBANK17		= 17,
	FBANK18		= 18,
	FBANK19		= 19,
	FBANK20		= 20,
	FBANK21		= 21,
	FBANK22		= 22,
	FBANK23		= 23,
	FBANK24		= 24,
	FBANK25		= 25,
	FBANK26		= 26,
	FBANK27		= 27,
}FilterBanks_CAN2;

typedef enum{
	FIFO0_CAN2	= 0,
	FIFO1_CAN2	= 1,
}FIFOs_CAN2;

#endif
