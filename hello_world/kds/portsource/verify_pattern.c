#include "verify_pattern.h"
#include "fsl_debug_console.h"
#include "fsl_tpm_driver.h"
#include "fsl_tpm_hal.h"
#include "MKL25Z4.h"
#include "MKL25Z4_extension.h"

#define UNIT_TIME 	(20.83)
#define TIME_IN_SEC	(1000000000)

extern uint32_t user_specified_seed_value;
extern uint32_t number_of_memory_blocks;
extern uint32_t number_of_allcation_instance;
extern uint32_t pattern_blocks_write;

/**
* @brief This function verifies the pseudo generated pattern at the location
* specified the user and upto the address specified by the user. It asks for the user
* input valid address and for number of blocks upto which user wants to verify the pattern.
* this function also calculates time taken to verify the pattern
*
* @param void
*
* @return integer value 2 for main menu 3 for exit
*/

uint32_t verify_pattern()
{
#ifdef HOST_PLATFORM
    clock_t time;
#endif
	uint32_t counter_ticks = 0;


    uint8_t index = 0;
    uint32_t ans = 0;
    uint32_t verify_seed = 0;
    uint8_t back_space;
    uint32_t offset_value = 0;
    uint32_t address_value = 0;
    uint32_t pattern_blocks = 0;
    uint32_t* memory_location = 0;
    uint32_t* temp_memory_ptr = ptrToMemory;

    //check for allocated memory first, if yes then go ahead and verify else skis the operation
    if(number_of_allcation_instance > 0)
    {
        PRINTF("\n\rAvailable range of memory addresses is from %d to %d", 0, number_of_memory_blocks - 1);
        PRINTF("\n\rFirst Address is: %x", temp_memory_ptr);
        PRINTF("\n\rLast Address is: %x ", (temp_memory_ptr + number_of_memory_blocks));
        PRINTF("\n\r");
        PRINTF("\n\rPlease enter Nth block number or address where you would like to enter the data: ");

        //asks user to select mode of address offset or hexadecimal

        uint8_t mode_of_address = address_mode_selection();

        switch(mode_of_address)
        {
        // perform the verify operation for offset value
        //TO DO: convert into function for both cases
        case 1:
        	PRINTF("\n\rEnter Your Offset Value: ");
            SCANF("%ld", &offset_value);
            if(offset_value >= 0 && offset_value < number_of_memory_blocks)
            {
                memory_location = (uint32_t*)(assign_address(offset_value));

                verify_seed = user_specified_seed_value;

                PRINTF("\n\rEnter the number of blocks upto which you would like to verify the pattern:");
                SCANF("%d", &pattern_blocks);
                if(pattern_blocks > pattern_blocks_write)
                {
                	PRINTF("\n\rCan't verify more than pattern generated blocks");
                    back_space = ask_backspace();
                    return back_space;
                }
#ifdef HOST_PLATFORM
                time = clock();
#endif

            	// Start LPTMR
            	LPTMR_DRV_Start(LPTMR0_IDX);

                for(index = 0; index < pattern_blocks; index++)
                {
                    ans = ((MULTIPLY * verify_seed)+CONSTANT)%POWER;
                    verify_seed = *(memory_location + index);

                    //if pattern not verified then prints actual data
                    if(ans != *(memory_location + index))
                    {
                    	PRINTF("\n\rActual Value: %d", ans);
                    	PRINTF("\n\rExpected Value: %d", *(memory_location + index));
                    	PRINTF("\n\rMemory Location: %x", (memory_location + index));
                        back_space = ask_backspace();
                        return back_space;
                    }
                }
                PRINTF("Successfully Verified the pattern");
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
            	PRINTF("\n\rEnter valid offset value.");
            }
            break;

        //perform verify operation for hexadecimal value
        case 2:
        	PRINTF("\n\rEnter Valid Address Value:");
        	SCANF("%x", &address_value);
            if(check_bound_for_address(address_value))
            {
                memory_location = (uint32_t*)address_value;

                verify_seed = user_specified_seed_value;

                PRINTF("\n\rEnter the number of blocks upto which you would like to verify the pattern:");
                SCANF("%d", &pattern_blocks);
                if(pattern_blocks > pattern_blocks_write);
                {
                	PRINTF("\n\rCan't verify more than pattern generated blocks");
                    back_space = ask_backspace();
                    return back_space;
                }
#ifdef HOST_PLATFORM
                time = clock();
#endif

            	// Start LPTMR
            	LPTMR_DRV_Start(LPTMR0_IDX);

                for(index = 0; index < pattern_blocks; index++)
                {
                	PRINTF("seed value %d", verify_seed);
                    ans = ((MULTIPLY * verify_seed)+CONSTANT)%POWER;
                    verify_seed = *(memory_location + index);

                    if(ans != *(memory_location + index))
                    {
                    	PRINTF("\n\rActual Value: %d", ans);
                    	PRINTF("\n\rExpected Value: %d", *(memory_location + index));
                    	PRINTF("\n\rMemory Location: %x", (memory_location + index));
                        back_space = ask_backspace();
                        return back_space;
                    }
                }
                PRINTF("Successfully Verified the pattern");
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
            	PRINTF("\n\rPlease Enter valid address");
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

    // asks for main menu or exit
    back_space = ask_backspace();
    return back_space;
}
