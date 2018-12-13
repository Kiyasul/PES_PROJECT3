/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
// SDK Included Files
#include "board.h"
#include "fsl_lptmr_driver.h"
#include "MKL25Z4.h"
#include "fsl_tpm_driver.h"
#include "fsl_debug_console.h"
#include "uart_driver.h"
#include "fsl_adc16_driver.h"
#include "fsl_dma_driver.h"
#include "dma_driver.h"
#include "help.h"
////////////////////////////////////////////////////////////////////////////////
// Project definitions
////////////////////////////////////////////////////////////////////////////////
//#define PROJECT1_IMM
//#define PROJECT2_UART
//#define BLOCKING_MODE
//#define PROJECT3_ADC_DMA_PART1	// PART1 is ADC Polling
#define PROJECT3_ADC_DMA
#if defined(PROJECT3_ADC_DMA)
#define PROJECT3_ADC_DMA_PART2		// PART2 is DMA with interrupts
#define ADC_0                   (0U)
#define CHANNEL_0               (0U)
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
// Timer period: 500000uS
#define TMR_PERIOD                          (500000U)
#define TX_BUFFER_LEN						(256)
#define RX_BUFFER_LEN						(64)

//#define BLOCKING_MODE
#if defined(TWR_KV46F150M)
#define LPTMR0_IDX LPTMR_IDX
#endif

uint8_t* message = 0;
uint16_t absolute_peak = 0;
volatile char chara = '\0';
// ADC RESULT VARIABLE
static int16_t adc_raw_value = 0;
static int index = 0;
DECAY_t decay;
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief LPTMR interrupt call back function.
 * The function is used to toggle LED1.
 */
void lptmr_call_back(void)
{
    // Toggle LED1
    LED1_TOGGLE;
}

//
/*!
 * @brief Main function
 */
int main (void)
{

#ifdef PROJECT2_UART
    // UART HANDLE
    UART_handle_t uart_handle  = {

    	.status = UART_STATE_RESET,
		.rx_buffer_size = RX_BUFFER_LEN,
		.tx_buffer_size = TX_BUFFER_LEN,
    	.baudrate = BAUDRATE_38400
    };

	// UART INIT
	UART_init(&uart_handle);
#endif


/****************** START OF PROJECT 3 *****************************/
/****************** START OF PROJECT 3 *****************************/
#ifdef PROJECT3_ADC_DMA

	hardware_init();

	// Initialize LED1
    LED1_EN;

#ifdef PROJECT1_IMM
	welcome_screen();
	user_menu();
#endif

	// ADC HANDLE
	adc16_converter_config_t adcUserConfig  = {

    	.lowPowerEnable = false,
		.clkDividerMode = kAdc16ClkDividerOf1,
		.longSampleTimeEnable = false,
		.resolution = kAdc16ResolutionBitOfDiffModeAs16,
		.clkSrc = kAdc16ClkSrcOfBusClk,
		.asyncClkEnable = false,
		.highSpeedEnable = true,
		.longSampleCycleMode = kAdc16LongSampleCycleOf6,
		.hwTriggerEnable = false,
		.refVoltSrc = kAdc16RefVoltSrcOfVref,
		.continuousConvEnable = true,
#ifdef PROJECT3_ADC_DMA_PART1
		.dmaEnable = false
#endif

#ifdef PROJECT3_ADC_DMA_PART2
		.dmaEnable = true
#endif
    };
	adc16_hw_average_config_t adcHWConfig = {

		.hwAverageEnable = true,
		.hwAverageCountMode = kAdc16HwAverageCountOf4
	};
	adc16_chn_config_t adcChnConfig = {

		.chnIdx = kAdc16Chn0d,
		.convCompletedIntEnable = false,
		.diffConvEnable = true
	};

#ifdef PROJECT3_ADC_DMA_PART2

	DMA0_DRV_Init();
#endif

	// ADC POLLING INIT
	if(ADC16_DRV_Init(ADC_0, &adcUserConfig) != kStatus_ADC16_Success)
		PRINTF("Unsuccessful to do the ADC init\r\n");
	if(ADC16_DRV_ConfigHwAverage(ADC_0, &adcHWConfig) != kStatus_ADC16_Success)
		PRINTF("Unsuccessful to do the ADC init\r\n");

	ADC16_DRV_ConfigConvChn(ADC_0, CHANNEL_0, &adcChnConfig);
	PRINTF("Successful to do the ADC init\r\n");


#endif

	decay.peak_decay_previous = 0;
	decay.peak_decay_factor = 0.8;
    while(1)
    {
#ifdef BLOCKING_MODE
		   /*wait till data register is empty*/
		   while((UART0_S1 & UART_S1_RDRF_MASK) == 0);
    	   chara = UART0->D;
    	   if((UART0_S1 & UART0_S1_TDRE_MASK ) && (UART0_S1 & UART0_C2_TIE_MASK)) {
    	    	UART0->D = chara;
    	    }
#endif

/****************** START OF PROJECT 3 *****************************/
/****************** START OF PROJECT 3 *****************************/
#ifdef PROJECT3_ADC_DMA

#ifdef PROJECT3_ADC_DMA_PART1

    	   ADC16_DRV_WaitConvDone(ADC_0,CHANNEL_0);
    	   adc_raw_value = ADC16_DRV_GetConvValueRAW(ADC_0, CHANNEL_0);
    	   PRINTF("The raw ADC value = %d\r\n", adc_raw_value);
#endif

#ifdef PROJECT3_ADC_DMA_PART2

   		if(schedule_event > 0) {

			/* Peak value calculation */
			absolute_peak = PEAK_Calculation(value);
			PRINTF("ABS CURRENT PEAK = %d\n", absolute_peak);

   			/* Decay value calculation */
			DECAY_AND_LOGARITHMIC_Calculation(absolute_peak);

   			/* Clear the flag again */
   			schedule_event = 0;
   		}
#endif
#endif
    }
    return 0;
}
