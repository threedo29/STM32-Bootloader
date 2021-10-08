#include "q_buffer.h"

serialQ_t* create_serialQ(void)
{
    serialQ_t* q;
    q = (serialQ_t*)malloc(sizeof(serialQ_t));
    q->init_pointer = &q->data[0];
    q->end_pointer = &q->data[MAX_DATA_SIZE - 1];
    q->in_pointer = &q->data[0];
    q->out_pointer = &q->data[0];

    return q;
}

/**
 * @brief   MAX_DATA_SIZE를 넘어가면 처음부터 덮어씀
 * 
 * @param   q       
 * @param   data    
 */
void put_serialQ(serialQ_t* q, unsigned char data)
{
    *(q->out_pointer) = data;

    if (q->out_pointer == q->end_pointer)
    {
        q->out_pointer = q->init_pointer;
    }
    else
    {
        q->out_pointer++;
    }
}

bool get_serialQ(serialQ_t* q, unsigned char* data)
{
    bool result = true;

    if (q->out_pointer == q->in_pointer)
    {
        result = false;
    }
    else
    {
        *data = *(q->in_pointer);
        if (q->in_pointer == q->end_pointer)
        {
            q->in_pointer = q->init_pointer;
        }
        else
        {
            q->in_pointer++;
        }
    }

    return result;
}

linked_queue_t* create_linkedQ(void)
{
    linked_queue_t* q;
    q = (linked_queue_t*)malloc(sizeof(linked_queue_t));
    q->front = NULL;
    q->rear = NULL;
    q->length = 0;

    return q;
}

bool is_empty_linkedQ(linked_queue_t* q)
{
    bool result = false;

    if (q->length == 0)
    {
        return true;
    }

    return result;
}

void enqueue_linkedQ(linked_queue_t* q, element_t item)
{
    queue_node_t* new_node;
    new_node = (queue_node_t*)malloc(sizeof(queue_node_t));
    new_node->data = item;
    new_node->link = NULL;

    if (q->length == 0)
    {
        q->front = q->rear = new_node;
    }
    else
    {
        q->front->link = new_node;
        q->rear = new_node;
    }

    q->length++;
}

element_t dequeue_linkedQ(linked_queue_t* q)
{
    queue_node_t* temp = NULL;
    element_t item;

    if (q->length == 0)
    {
        memset(&item, 0, sizeof(element_t));
    }
    else
    {
        item = q->front->data;
        temp = q->front;
        q->front = q->front->link;

        if (q->front == NULL)
        {
            q->rear = NULL;
        }
        q->length--;
        free(temp);
        temp = NULL;
    }
    return item;
}

void delete_linkedQ(linked_queue_t* q)
{
    queue_node_t* temp = NULL;

    if (q->length != 0)
    {
        temp = q->front;
        q->front = q->front->link;

        if (q->front == NULL)
        {
            q->rear = NULL;
        }

        q->length--;
        free(temp);
        temp = NULL;
    }
}

element_t peek(linked_queue_t* q)
{
    element_t item;
    if (q->length == 0)
    {
        memset(&item, 0, sizeof(element_t));
    }
    else
    {
        item = q->front->data;
    }
    return item;
}
