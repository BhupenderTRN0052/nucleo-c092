#include "queue.h"
#include <string.h>



void Queue_Init(Queue *queue)
{
    queue->head = 0;
    queue->tail = 0;
}

int enqueue(Queue *queue, CAN_Message *msg)
{
    uint8_t next = (queue->head + 1) % QUEUE_SIZE;

    if (next == queue->tail)
    {
        // Queue is full
        return -1;
    }

    queue->buffer[queue->head] = *msg;  // Copy message to the buffer
    queue->head = next;
    
    return 0;
}

int dequeue(Queue *queue, CAN_Message *msg)
{
    if (queue->head == queue->tail)
    {
        // Queue is empty
        return -1;
    }

    *msg = queue->buffer[queue->tail];  // Copy message from the buffer
    queue->tail = (queue->tail + 1) % QUEUE_SIZE;
    
    return 0;
}

int isEmpty(Queue *queue)
{
    return (queue->head == queue->tail);
}

int isFull(Queue *queue)
{
    return ((queue->head + 1) % QUEUE_SIZE == queue->tail);
}
