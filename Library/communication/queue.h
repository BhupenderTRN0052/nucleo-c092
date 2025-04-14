#ifndef QUEUE_H
#define QUEUE_H

#include "main.h"

#define QUEUE_SIZE 50


// Struct to hold CAN message
typedef struct {
    FDCAN_TxHeaderTypeDef TxHeader;
    FDCAN_RxHeaderTypeDef RxHeader;
    uint8_t Data[8];
} CAN_Message;

// Struct to hold the queue
typedef struct {
    CAN_Message buffer[QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
} Queue;

// Queue functions
void Queue_Init(Queue *queue);
int enqueue(Queue *queue, CAN_Message *msg);
int dequeue(Queue *queue, CAN_Message *msg);
int isEmpty(Queue *queue);
int isFull(Queue *queue);
#endif  // QUEUE_H