/*
 * dma_driver.h
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

#ifndef PORTHEADER_DMA_DRIVER_H_
#define PORTHEADER_DMA_DRIVER_H_

#include "MKL25Z4.h"
#include "board.h"

#define  ADC_READS 		 (64)
#define  MSB_16BIT_CLEAR (1<<15)
#define  MAX(x,y)  		 ((x>y)?x:y)

// __attribute__ ((aligned (2))) __attribute__((section(".ARM.__at_0x20001000"))) linker file
extern int16_t value[ADC_READS];
volatile int schedule_event;

/* Structure for peak decay calculation */
typedef struct {

	uint16_t peak_decay_current;	/* Peak current value 		*/
	uint16_t peak_decay_previous;	/* Peak previous value 		*/

	float peak_decay_factor;		/* Peak decay factor 		*/

	uint16_t peak_decay_result;		/* Peak decay result value 	*/
}DECAY_t;

/* Lookup table for logarithmic calculation */
const double logarithmicLookup[] = {

		-0.90308998699,		// x = 0.125
		-0.60205999132,		// x = 0.25
		-0.42596873227,		// x = 0.375
		-0.30102999566,		// x = 0.5
		-0.20411998265,		// x = 0.625
		-0.1249387366,		// x = 0.75
		-0.05799194697,		// x = 0.875
		 0,					// x = 1
};

uint16_t PEAK_Calculation(int16_t sample[]);
void LOGARITHMIC_Calculation(uint16_t);
void DECAY_AND_LOGARITHMIC_Calculation(uint16_t);
void DMA0_DRV_Init(void);
void DMA0_IRQHandler(void);


#endif /* PORTHEADER_DMA_DRIVER_H_ */
