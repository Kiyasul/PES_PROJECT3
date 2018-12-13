/*
 * uart.h
 *
 *  Created on: Nov 16, 2018
 *      Author: kiyasul
 */

#ifndef PORTHEADER_UART_H_
#define PORTHEADER_UART_H_

/*****************************************************/
/**********************Defines************************/
/*****************************************************/
/* following defines should be used for structure members */
#define     __IM     volatile const  /*! Defines 'read only' structure member permissions */
#define     __OM     volatile		 /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile		 /*! Defines 'read/write' structure member permissions */


#define UART0_BASE_ADDRESS 	(0x4006A000)
#define UART0				((UART0_Typedef*) UART0_BASE_ADDRESS)

typedef struct {





}UART0_Typedef;




















#endif /* PORTHEADER_UART_H_ */
