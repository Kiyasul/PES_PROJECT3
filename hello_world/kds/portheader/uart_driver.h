/*
 * uart_driver.h
 *
 *  Created on: 13-Nov-2018
 *      Author: hardi
 */


#include "circular_buffer.h"

#ifndef DRIVERS_UART_DRIVER_H_
#define DRIVERS_UART_DRIVER_H_
#define TX_EVENT  (0x1)
#define RX_EVENT  (0x2)
#define ARRAY_ROWS							(256)
#define ARRAY_COLUMNS						(2)

uint8_t volatile scheduled_event = 0;

/*UART status */
typedef enum
{
	UART_STATE_RESET,			/*UART is not yet initialized*/
	UART_STATE_READY,			/*UART is initialized and ready to use*/
	UART_STATE_BUSY,			/*UART is busy doing a process*/
	UART_STATE_TX,				/*UART is busy in data transmission*/
	UART_STATE_RX				/*UART is busy in data reception*/
}UART_status;

/*UART baudrate values*/
typedef enum
{
	BAUDRATE_115200 = 115200,
	BAUDRATE_57200  = 57200,
	BAUDRATE_38400  = 38400,
	BAUDRATE_9600   = 9600,
	BAUDRATE_4800   = 4800,
	BAUDRATE_2400   = 2400,
	BAUDRATE_300    = 300,
}UART_baudrate;

/*UART handle structure definition*/
typedef struct
{
	UART_status				status;					/*UART communication status*/
	circular_buffer_t		pUART_tx_buffer;		/*pointer to UART transmission buffer*/
	circular_buffer_t		pUART_rx_buffer;		/*pointer to UART receiver buffer*/
	uint16_t				tx_buffer_size;			/*transmission buffer size*/
	uint16_t				rx_buffer_size;			/*receiver buffer size*/
	UART_baudrate			baudrate;				/*baudrate setting*/
}UART_handle_t;

/**
 * @brief: API to initialize the UART peripheral
 * @param: *handle: pointer to the UART_handle_t structure
 * @reatval: void
 */
void UART_init(UART_handle_t *handle);


/*
 * @brief: API for UART data transmission
 * @param: *handle: pointer to UART_handle structure
 * @param: *buffer: pointer to the tx buffer
 * @param: length: length of the data
 * @retval: void
 */
void UART_send(uint8_t *send_char);
void UART_sendc(uint8_t tx_char);

/*
 * @brief: API for UART data reception
 * @param: *handle: pointer to UART_handle structure
 * @param: *buffer: pointer to rx buffer
 * @retval: void
 * */
void UART_receive(UART_handle_t *handle, uint8_t *buffer, uint32_t length);


/*
 *@brief - IRQ Handler
 *This is the IRQ Handler for UART. We handle two types of interrupts in this function
 *Receive Interrupts and Transmit interrupt.
 *Each interrupt should clear their associated flag when completed but only if they were set.
 *@param
 *@return - void
 */
void UART0_IRQHandler(void);

void ClearArray(int array[ARRAY_ROWS][ARRAY_COLUMNS]);
void PrintStatistics(int array[ARRAY_ROWS][ARRAY_COLUMNS]);
int * itoa(int data, int * ptr, int base);

#endif /* DRIVERS_UART_DRIVER_H_ */
