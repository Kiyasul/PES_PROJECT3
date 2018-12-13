/*
 * dma_driver.c
 *
 *  Created on: Dec 2, 2018
 *      Author: kiyasul
 *
 *
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

#include "dma_driver.h"
#include "fsl_debug_console.h"
#include "stdlib.h"
#include "math.h"

#define ADC_MAX_VALUE (32767)
/* Structure variable global */
extern DECAY_t decay;

/*
 * Absolute peak calculation function
 */
uint16_t PEAK_Calculation(int16_t sample[])
{
    int i;

    // Initialize maximum element
    uint16_t peak_value = abs(sample[0]);

    // Traverse array elements from second and
    // compare every element with current max
    for (i = 1; i < ADC_READS; i++) {
        if (abs(sample[i]) > peak_value)
        	peak_value = abs(sample[i]);
    }

    return peak_value;
}

/*
 * Logarithmic peak calculation function
 */
void LOGARITHMIC_Calculation(uint16_t log_value)
{

	/* Find appropriate index for the lookup table */
	int index = 0;
	double find_index = (double)((double)log_value/(double)ADC_MAX_VALUE);
	if(find_index < 0.125) {
		index = 0;
	} else if(find_index > 0.125 && find_index < 0.25) {
		index = 1;
	} else if(find_index > 0.25 && find_index < 0.375) {
		index = 2;
	} else if(find_index > 0.375 && find_index < 0.5) {
		index = 3;
	} else if(find_index > 0.5 && find_index < 0.625) {
		index = 4;
	} else if(find_index > 0.625 && find_index < 0.75) {
		index = 5;
	} else if(find_index > 0.75 && find_index < 0.875) {
		index = 6;
	} else {
		index = 7;
	}

	double log_sign = (double)20 * logarithmicLookup[index];
	PRINTF("LOGARITHMIC VALUE = %0.10lf\n", log_sign);
}


/**
 * Decay Part5: application calculation
 */
void DECAY_AND_LOGARITHMIC_Calculation(uint16_t absolute_peak)
{
	/* Initialize state */
	decay.peak_decay_current = absolute_peak;

	if(decay.peak_decay_previous == 0) {

		/* On Boot */
		PRINTF("PEAK WITH DECAY = % d\n", decay.peak_decay_current);
		/* Logarithmic calculation */
		LOGARITHMIC_Calculation(decay.peak_decay_current);

	} else {

		/* Project3 running */
		if(decay.peak_decay_current < decay.peak_decay_previous) {

			decay.peak_decay_result = (uint16_t)(decay.peak_decay_current * decay.peak_decay_factor);
			PRINTF("PEAK WITH DECAY = % d\n", decay.peak_decay_result);
   			/* Logarithmic calculation */
   			LOGARITHMIC_Calculation(decay.peak_decay_result);

		} else {

			PRINTF("PEAK WITH DECAY = % d\n", decay.peak_decay_current);
   			/* Logarithmic calculation */
   			LOGARITHMIC_Calculation(decay.peak_decay_current);
		}
	}

	/* Update state */
	decay.peak_decay_previous = decay.peak_decay_current;
}

/**
 * DMA Initialize
 */
void DMA0_DRV_Init(void)
{
	schedule_event = 0;

	// Enable clocks
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

	// Config DMA Mux for ADC operation
	// Disable DMA Mux channel first
	DMAMUX0_CHCFG0 = 0x00;				  // Clear before configuration

	// Configure DMA
	DMA_SAR0 = (uint32_t)&ADC0_RA;		  // SAR - Source address
	DMA_DAR0 = (uint32_t)&value;		  // DAD - Destination address
	DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(128);  // 2 bytes (16 bits) per transfer

	DMA_DCR0 |= (DMA_DCR_EINT_MASK|		// Enable interrupt
				 DMA_DCR_ERQ_MASK |		// Enable peripheral request
				 DMA_DCR_CS_MASK  |		// Enable cycle steal
				 DMA_DCR_SSIZE(2) |		// Set source size to 16 bits
				 DMA_DCR_DINC_MASK|		// Set increments to destination address
				 DMA_DCR_DMOD(4)  |     // Destination address modulo of 128 bytes
				 DMA_DCR_DSIZE(2));		// Set destination size of 16 bits

	// Enable DMA channel and set ADC0 as source
	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(40);

	// Enable interrupt
	NVIC_EnableIRQ(DMA0_IRQn);
}

/*
 * Handles DMA0 interrupt
 * Resets the BCR register and clears the DONE flag
 * */
void DMA0_IRQHandler(void)
{
	/* Toggle LED for frequency detection */
	LED1_TOGGLE;

	/* Enable DMA0 to run continuously */
	DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;		// Clear Done Flag
	DMA_DSR_BCR0 |= DMA_DSR_BCR_BCR(128);		// Set byte count register

	/* Scheduler */
	++schedule_event;							// scheduler flag
}
