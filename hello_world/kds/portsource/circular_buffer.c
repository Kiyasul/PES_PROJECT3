#include "circular_buffer.h"

#define CB_SIZE                 (5)


uint8_t circular_buffer_total_elements(circular_buffer_t *c_buffer) {

   if(c_buffer->buffer == NULL) {
       return NULL_POINTER_ERROR;
   }
    return c_buffer->element_count;
}

error_circular_buffer_t circular_buffer_full(circular_buffer_t *c_buffer)
{
    if(c_buffer->element_count == c_buffer->buffer_size)
    //if((c_buffer->head - c_buffer->tail) == c_buffer->buffer_size - 1)
    {
        //printf("Buffer is Full\n");
        return BUFFER_FULL_ERROR;
    }
    else
    {
        return NO_ERROR;
    }
}

error_circular_buffer_t circular_buffer_empty(circular_buffer_t *c_buffer)
{
    if(c_buffer->element_count == 0)
    //if((c_buffer->head - c_buffer->tail) == 0)
    {
        //printf("Buffer is empty\n");
        return BUFFER_EMPTY_ERROR;
    }
    else
    {
        return NO_ERROR;
    }
}

error_circular_buffer_t circular_buffer_delete_element(circular_buffer_t *c_buffer, uint8_t *data_to_delete)
{
    if(circular_buffer_empty(c_buffer) == BUFFER_EMPTY_ERROR)
    {
        return BUFFER_EMPTY_ERROR;
    }
    else
    {
        *(data_to_delete) = *(c_buffer->tail);
        //printf("Deleted data is: %d\n", *(c_buffer->tail));

        (c_buffer->element_count)--;

        if((c_buffer->tail - c_buffer->buffer) < ((c_buffer->buffer_size) - 1))
        {
            c_buffer->tail++;
            //printf("After deletion of data tail is: @ %p \n", c_buffer->tail);
        }
        else
        {
            c_buffer->tail = c_buffer->buffer;
            //printf("In else: After deletion of data tail is: @ %p \n", c_buffer->tail);
        }
    }

    return NO_ERROR;
}


error_circular_buffer_t circular_buffer_add_element(circular_buffer_t *c_buffer, uint8_t data_to_add)
{
    if(c_buffer->buffer == NULL)
    {
        return NULL_POINTER_ERROR;
    }

    if(data_to_add > 255 || data_to_add < 0)
    {
        return INVALID_DATA_ERROR;
    }

    if(circular_buffer_full(c_buffer) == BUFFER_FULL_ERROR)
    {
        return BUFFER_FULL_ERROR;
    }
    else
    {
        *(c_buffer->head) = data_to_add;
        //printf("data added is %d\n", *(c_buffer->head));
        (c_buffer->element_count)++;

        /*if head pointer points to the last memory location then roll it back
        to first memory location else move it to next location*/
        if((c_buffer->head - c_buffer->buffer) < ((c_buffer->buffer_size) - 1))
        {
            c_buffer->head = ++(c_buffer->head);
           // printf("After insertion of data head is: @ %p\n", c_buffer->head);
        }
        else
        {
            c_buffer->head = c_buffer->buffer;
            //printf("In else: After insertion of data head is: @ %p\n", c_buffer->head);
        }
    }

    return NO_ERROR;
}

error_circular_buffer_t circular_buffer_init(circular_buffer_t *c_buffer, uint16_t size)
{
    if(size > 256 || size < 0)
    {
        return BUFFER_SIZE_ERROR;
    }

    c_buffer->buffer = (uint8_t*)(malloc(sizeof(uint8_t) * size));

    if(c_buffer->buffer == NULL)
    {
        return NULL_POINTER_ERROR;
    }
    else
    {
        c_buffer->buffer_size = size;
        c_buffer->element_count = 0;
        c_buffer->head = c_buffer->buffer;
        c_buffer->tail = c_buffer->buffer;
        //printf("head is @%p and tail is @%p\n", c_buffer->head, c_buffer->tail);
    }

    return NO_ERROR;
}

