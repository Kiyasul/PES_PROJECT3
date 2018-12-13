#include "common.h"
#include "fsl_debug_console.h"
extern uint32_t number_of_memory_blocks;

/**
* @brief This function asks for user input for Going back to the main menu
* from the current menu or exit the application
*
* @param void
*
* @return returns user specified value for returning or exit
*/

uint32_t ask_backspace()
{
    //@to do Add third option for repeating the current operation
    uint32_t back_space = 0;


    uint32_t user_input = 0;
    char buffer_value;
    uint32_t validation_flag = 0;
    PRINTF("\n\r");
      /*Take User input and check the input is valid is not. asks for input again if
    use inputs invalid input*/
    do
    {
    	PRINTF("Go Back to User Menu or Exit\n\r");
    	PRINTF("\n\r");
    	PRINTF("2. User Menu\n\r");
    	PRINTF("3. Exit");
    	PRINTF("\n\r");
    	PRINTF("\n\rPlease Select Your Option: ");

    	if(SCANF("%d", &user_input) != 1)
        //if((SCANF("%d%c", &user_input, &buffer_value) != 2) || (buffer_value != '\n'))
        {
        	PRINTF("\n\rPlease Enter a valid Choice...");
        	PRINTF("\n\r");
            //while(getchar() != '\n');
            validation_flag = 0;
        }
        else
        {
            if((user_input == 2 || user_input == 3))
            {
                back_space = user_input;
                validation_flag = TRUE;
            }
            else
            {
            	PRINTF("\n\rPlease Enter a valid Choice...\n\r");
                //while(getchar() != '\n\r');
                validation_flag = 0;
            }
        }

    }
    while(validation_flag == FALSE);


    PRINTF("\n\r");

    return back_space;
}


/**
* @brief This function asks for user input for selecting the mode of address input,
* either offset value or the actual hexadecimal address.
*
* @param void
*
* @return returns user specified value for returning or exit
*/

uint8_t address_mode_selection()
{
    uint8_t validation_flag = FALSE;
    uint32_t user_input = 0;
    //char buffer_value;

      /*Take User input and check the input is valid is not. asks for input again if
    use inputs invalid input*/
    do
    {
    	PRINTF("\n\r");
    	PRINTF("\n\rPlease Select Your preferred way to enter the address");
    	PRINTF("\n\r");
    	PRINTF("\n\r1. Enter Offset Value");
    	PRINTF("\n\r2. Enter Address Value");
    	PRINTF("\n\r");
    	PRINTF("\n\rEnter Your Choice: ");

    	if(SCANF("%d", &user_input) != 1)
        //if((SCANF("%d%c", &user_input, &buffer_value) != 2) || (buffer_value != '\n'))
        {
        	PRINTF("\n\rPlease Enter a valid Choice...\n\r");
           // while(getchar() != '\n');
            validation_flag = 0;
        }
        else
        {
            if((user_input == 1 || user_input == 2))
            {
                validation_flag = TRUE;
            }
            else
            {
            	PRINTF("\n\rPlease Enter a valid Choice...\n\r");
                validation_flag = 0;
            }
        }

    }
    while(validation_flag == FALSE);

    return user_input;
}


/**
* @brief This function assigns defines a address according to the
* offset value specified the user
*
* @param offset_value offset value given by the user to do the operation
* on the address
*
* @return returns pointer to the address specified by the user
*/

uint32_t* assign_address(uint32_t offset_value)
{
    return (ptrToMemory + offset_value);
}

/**
* @brief This function checks weather the specified address by the user is in
* the range of allocated address or not.
*
* @param address_from_user user specified address
*
* @return returns integer value, 1 if address is in the range 0 otherwise
*/

uint8_t check_bound_for_address(uint32_t* address_from_user)
{
    uint32_t* temp_ptr = ptrToMemory;
    uint32_t index = 0;
    uint8_t verified_flag = 0;
    //check for the address if it is in the allocated memory range
    for(index = 0; index < number_of_memory_blocks; index++)
    {
        if((uint32_t*) address_from_user == temp_ptr)
        {
            verified_flag = TRUE;
        }
        else
        {
            temp_ptr++;
        }
    }

    if(verified_flag)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

