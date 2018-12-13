#include "free.h"
#include "fsl_debug_console.h"

extern uint32_t number_of_allcation_instance;

/**
* @brief This function deallocates memory allocated previously
*
* @param void
*
* @return returns integer value, 2 for back menu 3 for exit
*/

uint32_t free_memory()
{
    uint32_t back_space = 0;

    if(number_of_allcation_instance > 0)
    {
        free(ptrToMemory);
        PRINTF("\n\rMemory deallocated successfully!!");
        number_of_allcation_instance--;
    }
    else
    {
    	PRINTF("Not enough memory to deallocate!!\n\r");
    	PRINTF("Please allocate memory first before deallocating it!!\n\r");
    }

    //asks user input for return or exit
    back_space = ask_backspace();
    return back_space;

    return 0;
}
