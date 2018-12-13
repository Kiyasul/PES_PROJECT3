#include "invert.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_lptmr_driver.h"
#include "fsl_tpm_driver.h"
#include "fsl_tpm_hal.h"
#include "MKL25Z4.h"
#include "MKL25Z4_extension.h"

#define UNIT_TIME 	(20.83)
#define TIME_IN_SEC	(1000000000)

extern uint32_t number_of_memory_blocks;
extern uint32_t number_of_allcation_instance;

/**
* @brief This function

* @param user_choice user specified value to call the function
*
* @return void
*/

uint32_t invert_block()
{
    uint8_t back_space = 0;
    uint32_t offset_value = 0;
    uint32_t address_value = 0;
    uint32_t number_of_bytes = 0;
    uint32_t* memory_location = 0;
    uint32_t* temp_memory_ptr = ptrToMemory;

    if(number_of_allcation_instance > 0)
    {
        PRINTF("\n\rAvailable range of memory addresses is from %d to %d", 0, number_of_memory_blocks - 1);
        PRINTF("\n\rFirst Address is: %x", temp_memory_ptr);
        PRINTF("\n\rLast Address is: %x ", (temp_memory_ptr + number_of_memory_blocks));
        PRINTF("\n\r");
        PRINTF("\n\rPlease enter Nth block number or address where you would like to INVERT the data: ");
        uint8_t mode_of_address = address_mode_selection();

        switch(mode_of_address)
        {
        case 1:
        	PRINTF("\n\rEnter Your Offset Value: ");
            SCANF("%ld", &offset_value);
            if(offset_value >= 0 && offset_value < number_of_memory_blocks)
            {
                memory_location = (uint32_t*)(assign_address(offset_value));
                PRINTF("\n\rPlease enter how many number of blocks you want to invert: ");
                SCANF("%ld", &number_of_bytes);
                //printf("Before Inverted: %ld", *memory_location);
                inver_bits(memory_location, number_of_bytes);
                //printf("After Inversion: %ld", *memory_location);
                PRINTF("\n\rSuccessfully Inverted");
                PRINTF("\n\r");
            }
            else
            {
            	PRINTF("\n\rPlease Enter valid offset value");
            }
            break;

        case 2:
        	PRINTF("\n\rEnter Valid Address Value:");
        	SCANF("%x", &address_value);
            if(check_bound_for_address(address_value))
            {
                memory_location = (uint32_t*)address_value;
                PRINTF("\n\rPlease enter how many number of bytes you want to invert: ");
                SCANF("%d", &number_of_bytes);
                //printf("Before Inverted: %ld", *memory_location);
                inver_bits(memory_location, number_of_bytes);
                //printf("After Inversion: %ld", *memory_location);
                PRINTF("\n\rSuccessfully Inverted");
                PRINTF("\n\r");

            }

            break;

        default:
            break;
        }
    }
    else
    {
    	PRINTF("\n\rNot enough allocated memory to perform the operation.");
    	PRINTF("\n\rPlease Allocate memory first before using this function.");
    }

    back_space = ask_backspace();
    return back_space;

}

/**
* @brief This function inverts the data at the user specified location upto
* the length specified by the user. It also counts time taken by the system to perform the
* invert operation

* @param address pointer to the address where user wants to invert the data
*        bytes   number of bytes user wants to invert

* @return void
*/

void inver_bits(uint32_t* address, uint32_t bytes)
{
	uint32_t counter_ticks = 0;

	// Start LPTMR
	LPTMR_DRV_Start(LPTMR0_IDX);

	#ifdef HOST_PLATFORM
    clock_t time;
    time = clock();
	#endif

    //invert the data
    for (int i = 0; i<=bytes; i++)
    {
        *(address+i)  ^=  ~(0U);
    }

	#ifdef HOST_PLATFORM
    time = clock() - time;

    //calculate time taken
    time_to_perform_task = ((double)time)/CLOCKS_PER_SEC;
    PRINTF("\n\rTotal time taken to perform Invert Operation is: %0.10lf \n\r", time_to_perform_task);
	#endif

    LPTMR_DRV_Stop(TPM0_IDX);
    counter_ticks = LPTMR_DRV_GetCurrentTimeUs(TPM0_IDX);
    PRINTF("\n\rTotal time taken to perform Invert Operation is in us : %d \n\r", counter_ticks);
}
