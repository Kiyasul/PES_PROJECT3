#include "allocate.h"
#include "fsl_debug_console.h"
#include <math.h>
#define MAX_MEMORY_SIZE             (pow(2,64))

uint32_t number_of_allcation_instance = 0;
uint32_t number_of_memory_blocks = 0;

/**
* @brief Allocates memory
*
* This function allocates asks user to specify the number of bytes required to
* allocate the memory. And calls call_malloc() function to allocate memory
*
* @param void
*
* @return integer value 2 for back 3 for exit
*/

uint32_t allocate_memory(void)
{
    uint8_t validation_flag = 0;
    uint8_t back_space = 0;

    /*Take User input and check the input is valid is not. asks for input again if
    use inputs invalid input*/
    do
    {
    	PRINTF("\n\rPlease Enter the Number of Blocks to be Allocated: ");
    	SCANF("%d", &number_of_memory_blocks);
        PRINTF("\n\r");

        if(number_of_memory_blocks > 0 && number_of_memory_blocks < MAX_MEMORY_SIZE)
        {
            call_malloc(number_of_memory_blocks);
            validation_flag = TRUE;
        }
        else
        {
        	PRINTF("Please Enter a valid Number...\n\r\n\r");
            //while(getchar() != '\n');
            validation_flag = 0;
        }

    }
    while(validation_flag == FALSE);


    back_space = ask_backspace();
    return back_space;
}

/**
* @brief
*
* This function allocates memory using malloc(). Memory is allocated
* according to the number of bytes specified by the user.
*
* @param number_of_memory_blocks number of bytes specified by the user
*
* @return void
*/

void call_malloc(uint32_t number_of_memory_blocks)
{

    ptrToMemory = malloc(number_of_memory_blocks * sizeof(uint32_t));

    if(ptrToMemory == NULL)
    {
    	PRINTF("ERROR: Memory Allocation Failed!!");
    }
    else
    {
    	PRINTF("Memory is allocated successfully!!");
    	PRINTF("\n\r");
        number_of_allcation_instance++;
    }
}
