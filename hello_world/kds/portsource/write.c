#include "write.h"
#include "fsl_debug_console.h"
extern uint32_t number_of_memory_blocks;
extern uint32_t number_of_allcation_instance;

/**
* @brief This function checks asks user to enter the address or offset where one wants
* to write the data and if user enters valid address location then writes the data
*
* @param void
*
* @return returns integer value, 2 for back menu 3 for exit
*
*/

uint32_t write_to_memory()
{
    uint8_t back_space = 0;

    uint32_t offset_value = 0;
    uint32_t address_value = 0;
    uint32_t* memory_location = 0;
    uint32_t data_to_write = 0;
    uint32_t* temp_memory_ptr = ptrToMemory;


    //check for allocated memory first, if yes then go ahead and verify else skis the operation
    if(number_of_allcation_instance > 0)
    {
        PRINTF("\n\rAvailable range of memory addresses is from %d to %d", 0, number_of_memory_blocks - 1);
        PRINTF("\n\rFirst Address is: %p", temp_memory_ptr);
        PRINTF("\n\rLast Address is: %p ", (temp_memory_ptr + number_of_memory_blocks));
        PRINTF("\n\r");
        PRINTF("\n\rPlease enter Nth block number or address where you would like to enter the data: ");

        //asks user to select mode of address offset or hexadecimal
        uint8_t mode_of_address = address_mode_selection();

        switch(mode_of_address)
        {
        // perform the write operation for offset value
        //TO DO: convert into function for both cases
        case 1:
        	PRINTF("\n\rEnter Your Offset Value: ");
            SCANF("%ld", &offset_value);
            if(offset_value >= 0 && offset_value < number_of_memory_blocks)
            {
                memory_location = assign_address(offset_value);
                PRINTF("\n\rPlease enter data you want to write to the specified memory location: ");
                SCANF("%ld", &data_to_write);

                *memory_location = data_to_write;

                PRINTF("\n\rSuccessfully written %d @ %p", *memory_location, memory_location);
                PRINTF("\n\r");
            }
            else
            {
            	PRINTF("\n\rEnter valid offset value");
            }
            break;

        case 2:
             // perform the write operation for address value
        //TO DO: convert into function for both cases
        	PRINTF("\n\rEnter Valid Address Value:");
        	SCANF("%x", &address_value);
            if(check_bound_for_address(address_value))
            {
                memory_location = (uint32_t*)address_value;
                PRINTF("\n\rPlease enter data you want to write to the specified memory location: ");
                SCANF("%d", &data_to_write);

                *memory_location = data_to_write;

                PRINTF("\n\rSuccessfully written %d @ %p", *memory_location, memory_location);
                PRINTF("\n\r");
            }
            else
            {
            	PRINTF("\n\rEnter Valid Address Value");
            }

            break;

        default:
            break;
        }

    }
    else
    {
    	PRINTF("\n\rNot enough memory available to write.");
    	PRINTF("\n\rPlease Allocate memory first before trying to write.");
    	PRINTF("\n\r");
    }

    //asks for main menu or exit
    back_space = ask_backspace();
    return back_space;
}
