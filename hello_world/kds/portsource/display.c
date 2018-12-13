#include "display.h"
#include "fsl_debug_console.h"
extern uint32_t number_of_memory_blocks;
extern uint32_t number_of_allcation_instance;

/**
* @brief This function checks asks user to enter the address or offset from which one wants
* to read the data and if user enters valid address location then reads the data displays it to the console
*
* @param void
*
* @return returns integer value, 2 for back menu 3 for exit
*/

uint32_t display_data()
{
    uint8_t back_space = 0;

    uint32_t offset_value = 0;
    uint32_t address_value = 0;

    uint32_t* memory_location = 0;
    uint32_t* temp_memory_ptr = ptrToMemory;

    /*dfirst check if memory is allocated or not to read the data. if allocated then go ahead with read otherwise skip the operation and
    asks user to select another function*/
    if(number_of_allcation_instance > 0)
    {
    	PRINTF("\n\rAvailable range of memory blcks is from %d to %d", 0, number_of_memory_blocks - 1);
    	PRINTF("\n\r");
    	PRINTF("\n\rFirst Address is: %x", temp_memory_ptr);
    	PRINTF("\n\r");
    	PRINTF("\n\rLast Address is: %x ", (temp_memory_ptr + number_of_memory_blocks));
    	PRINTF("\n\r");
    	PRINTF("\n\rPlease enter Nth block number or hexadecimal address from where you would like to read the data: ");
    	PRINTF("\n\r");
        uint8_t mode_of_address = address_mode_selection();

        switch(mode_of_address)
        {
        case 1:
        	PRINTF("\n\rEnter Your Offset Value: ");
            SCANF("%ld", &offset_value);
            if(offset_value >= 0 && offset_value < number_of_memory_blocks)
            {
                memory_location = (uint32_t*)(assign_address(offset_value));
            }
            else
            {
            	PRINTF("\n\r Please Enter valid offset value");
            }
            break;

        case 2:
        	PRINTF("\n\rEnter Valid Address Value:");
        	SCANF("%x", &address_value);
            memory_location = (uint32_t*)address_value;
            break;

        default:
            break;
        }

        if(*memory_location == NULL)
        {
        	PRINTF("\n\rEmpty memory Location. Please write data before reading from the location.");
        }
        else
        {
        	PRINTF("\n\rSuccessfully read %d @ %p", *memory_location, memory_location);
        	PRINTF("\n\r");
        }
    }
    else
    {
    	PRINTF("\n\rNot enough memory available to write.");
    	PRINTF("\n\rPlease Allocate memory first before trying to write.");
    }

    //asks user for return or exit
    back_space = ask_backspace();
    return back_space;
}
