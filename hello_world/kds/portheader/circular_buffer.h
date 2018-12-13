#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

typedef enum
{
    NO_ERROR,
    BUFFER_FULL_ERROR,
    BUFFER_EMPTY_ERROR,
    NULL_POINTER_ERROR,
    INVALID_DATA_ERROR,
    BUFFER_SIZE_ERROR

}error_circular_buffer_t;


typedef struct
{
    uint8_t* buffer;
    uint8_t* head;
    uint8_t* tail;
    uint16_t buffer_size;
    uint16_t element_count;
}circular_buffer_t;


/*
*@brief - get number of elements from the buffer
*@param - *c_buffer: pointer to circular buffer structure
*@return - error
*/
uint8_t circular_buffer_total_elements(circular_buffer_t *c_buffer);



/*
 *@brief - allocates memory for the buffer and initializes head and tail pointers
 *@param - *c_buffer: pointer to circular buffer structure
 *@param - size: size of the memory to be allocated for the circular buffer
 *@return - error
*/
error_circular_buffer_t circular_buffer_init(circular_buffer_t *c_buffer, uint16_t size);

/*
 *@brief - checks if circular buffer is full or not
 *@param - *c_buffer: pointer to circular buffer structure
 *@return - error
*/
error_circular_buffer_t circular_buffer_full(circular_buffer_t *c_buffer);

/*
 *@brief - checks if circular buffer is empty or not
 *@param - *c_buffer: pointer to circular buffer structure
 *@return - error
*/
error_circular_buffer_t circular_buffer_empty(circular_buffer_t *c_buffer);

/*
 *@brief - adds new element to the buffer
 *@param - *c_buffer: pointer to circular buffer structure
 *@param - data_to_add: new element to be added
 *@return - error
*/
error_circular_buffer_t circular_buffer_add_element(circular_buffer_t *c_buffer, uint8_t data_to_add);

/*
 *@brief - delete an element from the buffer
 *@param - *c_buffer: pointer to circular buffer structure
 *@param - *deta_deleted: removed element from the buffer
 *@return - error
*/
error_circular_buffer_t circular_buffer_delete_element(circular_buffer_t *c_buffer, uint8_t *data_deleted);









