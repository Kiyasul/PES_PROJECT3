#include "help.h"
#include "fsl_debug_console.h"

#define MAXSIZE		(10)

typedef uint32_t (*ptrToFunction)();

//lookup table for functions
struct
{
    ptrToFunction call_the_function;
    uint32_t function_id;
} function_lookup_table[MAXSIZE] = { {&help, 1},
    {&allocate_memory, 2},
    {&free_memory, 3},
    {&write_to_memory, 4},
    {&display_data, 5},
    {&invert_block, 6},
    {&write_pattern, 7},
    {&verify_pattern, 8},
    {&exit_UI, 9},
    {NULL, 0}
};

/**
* @brief This function takes user input for different types of function to run and checks for the valid input
* If input is not valid it asks to input again till user inputs valid value.
*
* @param void
*
* @return void
*/
void user_choice(void)
{
    uint32_t user_input = 0;
    char buffer_value;
    uint32_t validation_flag = 0;
    PRINTF("\n\r");

    PRINTF("Please Enter Your Choice of Operation followed by an Enter Key. \n\r\n\rFor example, Press \"1 for Help\" to know more. \n\r");

    do
    {
    	PRINTF("\n\rPlease Enter Your Choice: ");

    	if(SCANF("%d", &user_input) != 1)
        //if((SCANF("%d%c", &user_input, &buffer_value) != 2) || (buffer_value != '\n'))
        {
        	PRINTF("\n\rPlease Enter a valid Choice...\n\r");
            while(getchar() != '\n');
            validation_flag = 0;
        }
        else
        {
            if((user_input > 0 && user_input < 10))
            {
                lookup_table(user_input);
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
}

/**
* @brief This function display various functions to choose from
*
* @param void
*
* @return void
*/

void user_menu(void)
{
	PRINTF("Please Select an Option\n\r");
	PRINTF("\n\r");
	PRINTF("1. Help\n\r");
	PRINTF("2. Allocate Memory\n\r");
	PRINTF("3. Free Memory\n\r");
	PRINTF("4. Write Memory\n\r");
	PRINTF("5. Display Memory\n\r");
	PRINTF("6. Invert Block\n\r");
	PRINTF("7. Write Pattern\n\r");
	PRINTF("8. Verify Pattern\n\r");
	PRINTF("9. Exit");
	PRINTF("\n\r\n\r");
    user_choice();
}

/**
* @brief This function display welcome message for the application
*
* @param void
*
* @return void
*/

void welcome_screen(void)
{
	PRINTF("\n\r\n\r\n\r");
	PRINTF("***********************************************************************************\n\r");
	PRINTF("***********************************************************************************\n\r");
	PRINTF("*                                                                                 *\n\r");
	PRINTF("*                    WELCOME TO THE INTERACTIVE MEMORY MANAGEMENT                 *\n\r");
	PRINTF("*                                                                                 *\n\r");
	PRINTF("***********************************************************************************\n\r");
	PRINTF("***********************************************************************************\n\r");
	PRINTF("\n\r\n\r\n\r");
}


/**
* @brief This function display helps for various functions to choose from
*
* @param void
*
* @return 0 for successful
*/
uint32_t help(void)
{
	PRINTF("\n\r\n\r\n\r");
	PRINTF("*******************************************\n\r");
	PRINTF("                                           \n\r");
	PRINTF("       User Guide to Use the Menu          \n\r");
	PRINTF("                                           \n\r");
	PRINTF("*******************************************\n\r");
	PRINTF("\n\r\n\r");

	PRINTF("# Please select your choice and enter a valid integer number.\n\r");
	PRINTF("# for example, enter 4 and press Enter key for Write to Memory \n\r");
	PRINTF("# Invalid choices are 5., 5@, 5a, 5A, etc etc...  \n\r");

	PRINTF("\n\r\n\r");

	PRINTF("# Allocate function allocates user specified memory \n\r");
	PRINTF("# Free function deallocates previously allocated memory \n\r");
	PRINTF("# Write function writes a data specified by user to the user specified location \n\r");
	PRINTF("# Display function reads data from the user specified location");
	PRINTF("# Invert function inverts the data in the address specified by the user\n\r");
	PRINTF("# Write Pattern function generates pseudo random pattern to user specified location upto the length given by the user\n\r");
	PRINTF("# Verify Pattern verifies the previously generated pattern\n\r");

    user_menu();
    //user_choice();

    return 0;


}

/**
* @brief This function exits from the application
*
* @param void
*
* @return void
*/
uint32_t exit_UI(void)
{
	user_menu();
}

/**
* @brief This function calls the function from the  various functions to choose from
* as per the choice given by the user

* @param user_choice user specified value to call the function
*
* @return void
*/
void lookup_table(uint32_t user_choice)
{
    uint32_t index = 0;
    uint32_t continue_flag = 0;

    for(index = 0; index < 10; index++)
    {
        if(function_lookup_table[index].function_id == user_choice)
        {
            continue_flag = function_lookup_table[index].call_the_function();
        }
    }

    /*if user wants to continue using application then prints user menu
    exit fomr the application otherwise*/
    if(continue_flag == 2)
    {
        user_menu();
    }
    else
    {
        exit_UI();
    }
}

