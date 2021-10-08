#ifndef __Q_BUFFER_H__
#define __Q_BUFFER_H__

#include "common.h"

#define MAX_DATA_SIZE                               1024

typedef struct serialQ
{
    unsigned char data[MAX_DATA_SIZE];
    unsigned char* out_pointer;
    unsigned char* in_pointer;
    unsigned char* init_pointer;
    unsigned char* end_pointer;
} serialQ_t;

serialQ_t* create_serialQ(void);
void put_serialQ(serialQ_t* q, unsigned char data); // MAX_DATA_SIZE를 넘어가면 처음부터 덮어씀
bool get_serialQ(serialQ_t* q, unsigned char* data);

typedef struct element
{
    unsigned char data;
} element_t;

typedef struct queue_node
{
    element_t data;
    struct queue_node* link;
} queue_node_t;

typedef struct linked_queue
{
    queue_node_t* front;
    queue_node_t* rear;
    u32 length;
} linked_queue_t;

linked_queue_t* create_linkedQ(void);
bool is_empty_linkedQ(linked_queue_t* q);
void enqueue_linkedQ(linked_queue_t* q, element_t item);
element_t dequeue_linkedQ(linked_queue_t* q);
void delete_linkedQ(linked_queue_t* q);
element_t peek(linked_queue_t* q);

#endif /* __Q_BUFFER_H__ */