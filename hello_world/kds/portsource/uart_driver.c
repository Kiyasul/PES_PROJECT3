/*
 * uart_driver.c
 *
 *  Created on: 13-Nov-2018
 *      Author: hardi
 */
#include "board.h"
#include "uart_driver.h"
#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "fsl_debug_console.h"
#include "fsl_interrupt_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define BLOCKING_MODE
#define NON_BLOCKING_MODE

#define UART0_SRC_MCGFLLCLK					(1)
#define UART0_PLLFLLSEL_MCGFLLCLK			(0)
#define UART0_CLKGATE_EN					(1)
#define UART0_RXSRC							(0)
#define UART0_TXSRC							(0)
#define UART0_8BIT							(0)
#define UART0_PARITY_DISABLED				(0)
#define UART0_STOPBIT_1						(0)
#define UART0_OSR							(16)
#define UART0_CORE_CLOCK					(20970000)

#define ARRAY_ROWS							(256)
#define ARRAY_COLUMNS						(2)

int array[256][2];
volatile char received_char = '\0';

circular_buffer_t tx_buffer;
circular_buffer_t rx_buffer;

void UART_init(UART_handle_t *handle)
{
	/*setup receiver circular buffer*/
	if (circular_buffer_init(&rx_buffer, handle->rx_buffer_size) == NULL_POINTER_ERROR) {
		LED1_ON;
	}

	/*setup transmission circular buffer*/
	if(circular_buffer_init(&tx_buffer, handle->tx_buffer_size) == NULL_POINTER_ERROR) {
		LED1_ON;
	}

	ClearArray(array);

	/*Enable Port A clockgating and alternate functions*/
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	PORTA_PCR1 = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);
	PORTA_PCR2 = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);

	/*Select clock source for UART0, clocksource = FLL*/
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(UART0_SRC_MCGFLLCLK);
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL(UART0_PLLFLLSEL_MCGFLLCLK);

	/*Enable UART0 clock gating*/
	SIM_SCGC4 |= SIM_SCGC4_UART0(UART0_CLKGATE_EN);

	/*Select Tx and Rx pin*/
	SIM_SOPT5 |= SIM_SOPT5_UART0RXSRC(UART0_RXSRC);
	SIM_SOPT5 |= SIM_SOPT5_UART0TXSRC(UART0_TXSRC);

	/*Disbale Rx and Tx before enabling*/
	UART0_C2  = 0;
	UART0_C2 |= UART0_C2_RE(0) | UART0_C2_TE(0);

	/*Set data size, parity and stopbits
	 * data size = 8 bits, parity = no parity, stopbit = 1*/
	UART0_C1 |= UART0_C1_M(0) | UART0_C1_PE(0);
	UART0_BDH |= UART0_BDH_SBNS(UART0_STOPBIT_1);

	//Setting OSR bits to 0b01111 = 15, for OSR = 16
	UART0_C4 |= UART0_C4_OSR(UART0_OSR - 1);

	uint16_t SBR = ((UART0_CORE_CLOCK) / ((UART0_OSR) * (handle->baudrate)));
	UART0_BDH |= ((SBR >> 8) & UART0_BDH_SBR_MASK);
	UART0_BDL = ((SBR) & UART0_BDL_SBR_MASK);

	/* Enable transmission and reception */
	UART0_C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);

	handle->status = UART_STATE_READY;

#ifdef NON_BLOCKING_MODE

	/* Enable RIE and TCIE */
	UART0_C2 |= (UART0_C2_RIE_MASK | UART0_C2_TIE_MASK);
	/* Enable interrupt in NVIC */
	NVIC_EnableIRQ(UART0_IRQn);

#endif

}

void ClearArray(int array[ARRAY_ROWS][ARRAY_COLUMNS]) {

	   for(int i = 0; i < ARRAY_ROWS; i++) {

		   for(int j = 0; j < ARRAY_COLUMNS; j++) {

			   array[i][j] = 0;
	       }
	   }
}
void PrintStatistics(int array[ARRAY_ROWS][ARRAY_COLUMNS]) {

	   // Start in a new line
	   UART0->D = '\n';
	   for(int i = 0; i < ARRAY_ROWS; i++) {

		   for(int j = 0; j < ARRAY_COLUMNS; j++) {

			   if(array[i][j] > 0) {

				   int charac[10] = {0,0,0,0,0,0,0,0,0,0};
				   int *charac_ptr = charac;

				   int number[10] = {0,0,0,0,0,0,0,0,0,0};
				   int *number_ptr = number;

				   if(j == 0) {

					   UART_sendc((char)array[i][j]);
	               }
	               else {

	            	   UART0->D = '-';
	            	   number_ptr = itoa(array[i][j], charac_ptr, 10);
	            	   while(*number_ptr != '\0' ) {
	            		   UART_send((uint8_t *)number_ptr);
	            		   number_ptr++;
	            	   }
	            	   UART0->D = '\n';
	               }
	           }
	       }
	   }
}

void UART_send(uint8_t *send_char)
{
	//handle->status = UART_STATE_TX;

	__disable_irq();

	   /*block till transmit data register is empty*/
	   while(!(UART0_S1 & UART_S1_TDRE_MASK));
	   if(send_char != NULL) {

		    UART0->D  = *send_char;
		    /*wait till transmission completes*/
		    while(!(UART0_S1 & UART_S1_TC_MASK));
		}
	__enable_irq();
}
void UART_receive(UART_handle_t *handle, uint8_t *buffer, uint32_t length)
{
	handle->status = UART_STATE_RX;

	__disable_irq();
		/*wait till data register is empty*/
		while((UART0_S1 & UART_S1_RDRF_MASK) == 0);
		*buffer = UART0->D;
	__enable_irq();
}

#ifdef NON_BLOCKING_MODE
void UART_sendc(uint8_t tx_char)
{
	//handle->status = UART_STATE_TX;

	__disable_irq();

	   /*block till transmit data register is empty*/
	   while(!(UART0_S1 & UART_S1_TDRE_MASK));
	   if(tx_char != (int)NULL) {

		    UART0->D  = tx_char;
		    /*wait till transmission completes*/
		    while(!(UART0_S1 & UART_S1_TC_MASK));
		}
	__enable_irq();
}

void UART0_IRQHandler(void)
{
	__disable_irq();
	uint8_t transmit_data = 0;

		/* Receive condition */
	if((UART0_S1 & UART0_S1_RDRF_MASK) && (UART0_C2 & UART0_C2_RIE_MASK)) {

		received_char = UART0->D;

		/* If buffer is full */
		if(circular_buffer_full(&rx_buffer) == BUFFER_FULL_ERROR) {

			// Disable receive interrupts
			UART0->C2 &= ~(UART_C2_RIE(1));
		}
		else {

			if(received_char != '\n') {

				if(received_char == '\r') {

					PrintStatistics(array);
					ClearArray(array);
				}
				else {
					// Add element to the ring buffer
					circular_buffer_add_element(&rx_buffer, (uint8_t)received_char);
					int store = (int)received_char;
					array[store][0] = store;
					array[store][1]++;
				}
			}
			else {

				PrintStatistics(array);
				ClearArray(array);
			}
		}
	}
	/* Transmit condition */
	if((UART0_S1 & UART0_S1_TDRE_MASK ) && (UART0_S1 & UART0_C2_TIE_MASK)) {

		if(circular_buffer_empty(&rx_buffer) == BUFFER_EMPTY_ERROR) {

			// Disable transmit interrupts
			UART0->C2 &= ~(UART_C2_TCIE(1));
		}
		else {

			circular_buffer_delete_element(&rx_buffer, &transmit_data);
			UART0->D = transmit_data;
		}
	}
	__enable_irq();
}

int *itoa(int data, int * ptr, int base)
{
	if(ptr != NULL)
	{
		int i=0;
		int negative=0;
		int remain = 0;
		int l=0;
		char charval,tempVariable;
		int length;

		if(data == 0)				//checking if data is 0
		{
			*(ptr+i)='0';
			i++;
			*(ptr+i)='\0';
		}

		if ( data < 0 && base == 10)			//for negative numbers with base 10
		{
			data = data * (-1);
			negative = 1;
		}

		while (data != 0)
		{
			remain = data % base;		//gets the bits seperately using modulus operator
			if(remain <= 9)
			{
				charval = remain +'0';		//for 1 to 9 digits
			}
			else if(remain > 9)
			{
				charval = (remain-10)+'a';	//for hex values 'A' to 'F'
			}
			*(ptr+i) = charval;
			data = data / base;
			i++;
		}

		if(negative)				// for negative number
		{
			*(ptr+i) = '-';
			i++;
		}

		*(ptr+i)='\0';				// adding null character
		length=i;
		i=0;

		if(length%2 == 0) {
			l=length/2;
		}
		else if (length%2 !=0) {
			l = (length-1)/2;
		}
		while(i<l)				//swapping the positions of pointers
		{
			tempVariable=*(ptr+i);
			*(ptr+i)=*(ptr+length-i-1);
			*(ptr+length-i-1)=tempVariable;
			i++;
		}
		return ptr;
	}
	else
		return NULL;
}

#endif
