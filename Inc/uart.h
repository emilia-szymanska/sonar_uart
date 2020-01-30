/*
 * uart.h
 *
 *  Created on: 25.03.2019
 *      Author: Kurat
 */

#ifndef UART_H_
#define UART_H_

#include <stm32f3xx_ll_usart.h>

typedef enum {
	UART_READY = 0,
	UART_BUSY,
	UART_ERROR,
	UART_TIMEOUT
} UART_State;

typedef struct{

	USART_TypeDef *Instance;
	// Transmission
	UART_State tx_state;
	uint8_t *tx_buffer;
	uint32_t tx_buffer_length;

	// Reception
	UART_State rx_state;
	uint8_t *rx_buffer;
	uint32_t rx_buffer_length;
	uint32_t rx_received_bytes;

}UART_handler_t;

void _uart_it_callback_tx(UART_handler_t * huart);
void _uart_it_callback_rx(UART_handler_t * huart);


void uart_init(UART_handler_t *huart, USART_TypeDef *instance);

void uart_it_callback(UART_handler_t *huart);

void uart_transmit_it(UART_handler_t *huart, uint8_t *data, uint32_t length);
void uart_receive_line_it(UART_handler_t *huart, uint8_t *rx_buffer, uint32_t length);

UART_State uart_wait_for_tc(UART_handler_t *huart, uint32_t timeout);
UART_State uart_wait_for_rc(UART_handler_t *huart, uint32_t timeout);


#endif /* UART_H_ */
