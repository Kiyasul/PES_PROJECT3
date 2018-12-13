#include "writePattern.h"
#include "fsl_debug_console.h"
#include "fsl_tpm_driver.h"
#include "fsl_tpm_hal.h"
#include "MKL25Z4.h"
#include "MKL25Z4_extension.h"

#define UNIT_TIME 	(20.83)
#define TIME_IN_SEC	(1000000000)

extern uint32_t number_of_memory_blocks;
extern uint32_t number_of_allcation_instance;
uint32_t seed_value = 0;
uint32_t user_specified_seed_value = 0;
uint32_t pattern_blocks_write = 0;

/**
* @brief This function generates the pseudo generated pattern at the location
* specified the user and upto the address specified by the user. It asks for the user
* input valid address and for number of blocks upto which user wants to write the pattern.
* this function also calculates time taken to verify the pattern
*
* @param void
*
* @return integer value 2 for main menu 3 for exit
*/

uint32_t write_pattern()
{
#ifdef HOST_PLATFORM
    clock_t time;
#endif

	uint32_t counter_ticks;
    uint8_t index = 0;
    uint8_t back_space = 0;
    uint32_t offset_value = 0;
    uint32_t address_value = 0;
    uint32_t* memory_location = 0;
    uint32_t* temp_memory_ptr = ptrToMemory;

    //check for allocated memory first, if yes then go ahead and verify else skis the operation
    if(number_of_allcation_instance > 0)
    {
        PRINTF("\n\rAvailable range of memory addresses is from %d to %d", 0, number_of_memory_blocks - 1);
        PRINTF("\n\rFirst Address is: %u", temp_memory_ptr);
        PRINTF("\n\rLast Address is: %u", (temp_memory_ptr + number_of_memory_blocks));
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
                memory_location = (uint32_t*)(assign_address(offset_value));
                PRINTF("\n\rEnter the number of blocks upto which you would like to generate the pattern:");
                SCANF("%d", &pattern_blocks_write);
                PRINTF("Please Enter the seed value:");
                SCANF("%d", &seed_value);
                user_specified_seed_value = seed_value;
#ifdef HOST_PLATFORM
                time = clock();
#endif
            	// Start LPTMR
            	LPTMR_DRV_Start(LPTMR0_IDX);
                for(index = 0; index < pattern_blocks_write; index++)
                {
                    *(memory_location+index) = psuedo_generate(seed_value);
                    seed_value = *(memory_location + index);
                }
#ifdef HOST_PLATFORM
                time = clock() - time;
                time_to_perform_task = ((double)time)/CLOCKS_PER_SEC;
                PRINTF("\n\rTotal time taken to perform the Operation is: %0.10lf \n\r", time_to_perform_task);
#endif
                LPTMR_DRV_Stop(TPM0_IDX);
                counter_ticks = LPTMR_DRV_GetCurrentTimeUs(TPM0_IDX);
                PRINTF("\n\rTotal time taken to perform Invert Operation is in us : %d \n\r", counter_ticks);
            }
            else
            {
            	PRINTF("\n\rEnter Valid offset value.");
            }
            break;

        case 2:

             // perform the verify operation for offset value
            //TO DO: convert into function for both cases
        	PRINTF("\n\rEnter Valid Address Value:");
            SCANF("%x", &address_value);
            if(check_bound_for_address(address_value))
            {
                memory_location = (uint32_t*)address_value;
                PRINTF("\n\rEnter the number of blocks upto which you would like to generate the pattern:");
                SCANF("%d", &pattern_blocks_write);
                PRINTF("Please Enter the seed value:");
                SCANF("%d", &seed_value);
                user_specified_seed_value = seed_value;
#ifdef HOST_PLATFORM
                time = clock();
#endif
            	// Start LPTMR
            	LPTMR_DRV_Start(LPTMR0_IDX);
                for(index = 0; index < pattern_blocks_write; index++)
                {
                    *(memory_location+index) = psuedo_generate(seed_value);
                    seed_value = *(memory_location + index);
                }
#ifdef HOST_PLATFORM
                time = clock() - time;
                time_to_perform_task = ((double)time)/CLOCKS_PER_SEC;
                PRINTF("\n\rTotal time taken to perform Invert Operation is: %0.10lf \n\r", time_to_perform_task);
#endif
                LPTMR_DRV_Stop(TPM0_IDX);
                counter_ticks = LPTMR_DRV_GetCurrentTimeUs(TPM0_IDX);
                PRINTF("\n\rTotal time taken to perform Invert Operation is in us : %d \n\r", counter_ticks);
            }
            else
            {
            	PRINTF("Please Enter Valid address");
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
    }

    //asks for main menu or exit
    back_space = ask_backspace();
    return back_space;
}

/**
* @brief This function generates the random value at the user specified location
*
* @param void
*
* @return random integer value generated
*/

uint32_t psuedo_generate(uint32_t seed_value)
{
    uint32_t ans = 0;

    ans = ((MULTIPLY * seed_value)+CONSTANT)%POWER;
    return ans;
}
