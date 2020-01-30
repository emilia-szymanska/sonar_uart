/*
 * uart.c
 *
 *  Created on: 25.03.2019
 *      Author: Kurat
 */

#include <uart.h>

#define LL_MAX_DELAY                  0xFFFFFFFFU

void _uart_it_callback_tx(UART_handler_t *huart)
{
	if(huart->tx_buffer_length > 0)
	{
		huart->tx_buffer_length--;
		LL_USART_TransmitData8(huart->Instance, *(huart->tx_buffer++));
	}else{
		LL_USART_DisableIT_TXE(huart->Instance);
		CLEAR_BIT(huart->Instance->RQR, USART_RQR_TXFRQ);
		huart->tx_state = UART_READY;
	}
}
void _uart_it_callback_rx(UART_handler_t *huart) {
	if(huart->rx_state != UART_BUSY)
		return;

	uint8_t rx_byte = LL_USART_ReceiveData8(huart->Instance);

	(huart->rx_buffer[huart->rx_received_bytes++]) = rx_byte;

	if (rx_byte == '\n') {
		huart->rx_state = UART_READY;
		LL_USART_DisableIT_RXNE(huart->Instance);
	}else if(huart->rx_received_bytes >= huart->rx_buffer_length)
	{
		huart->rx_state = UART_ERROR;
		LL_USART_DisableIT_RXNE(huart->Instance);
	}
}


void uart_init(UART_handler_t *huart, USART_TypeDef *instance){
	huart->Instance = instance;

	huart->tx_state = UART_READY;
	huart->rx_state = UART_READY;


	huart->rx_buffer_length = 0;
	huart->rx_received_bytes = 0;

	LL_USART_EnableDirectionTx(huart->Instance);
	LL_USART_EnableDirectionRx(huart->Instance);

	LL_USART_Enable(huart->Instance);
}

void uart_it_callback(UART_handler_t *huart)
{
	if(LL_USART_IsActiveFlag_TXE(huart->Instance) && LL_USART_IsEnabledIT_TXE(huart->Instance))
	{
		_uart_it_callback_tx(huart);
	}
	if(LL_USART_IsActiveFlag_RXNE(huart->Instance))
	{
		LL_USART_ClearFlag_NE(huart->Instance);
		_uart_it_callback_rx(huart);
	}
}

void uart_transmit_it(UART_handler_t *huart, uint8_t *data, uint32_t length)
{
	huart->tx_state = UART_BUSY;
	huart->tx_buffer = data;
	huart->tx_buffer_length = length;

	LL_USART_EnableIT_TXE(huart->Instance);
}

void uart_receive_line_it(UART_handler_t *huart, uint8_t *rx_buffer,
		uint32_t length) {

	if (!length) {
		huart->rx_state = UART_ERROR;
		return;
	}

	huart->rx_state = UART_BUSY;

	huart->rx_buffer = rx_buffer;
	huart->rx_buffer_length = length;
	huart->rx_received_bytes = 0;
	LL_USART_EnableIT_RXNE(huart->Instance);
}


UART_State uart_wait_for_tc(UART_handler_t *huart, uint32_t timeout)
{
	  __IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
	  /* Add this code to indicate that local variable is not used */
	  ((void)tmp);

	  if(timeout == 0)
		  timeout = LL_MAX_DELAY;

	  while (huart->tx_state!=UART_READY && timeout)
	  {
	    if((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
	    {
	    	timeout--;
	    }
	  }

	  if(!timeout)
		  return UART_TIMEOUT;
	  else
		  return UART_READY;
}
UART_State uart_wait_for_rc(UART_handler_t *huart, uint32_t timeout)
{
	  __IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
	  /* Add this code to indicate that local variable is not used */
	  ((void)tmp);

	  if(timeout == 0)
		  timeout = LL_MAX_DELAY;

	  while (huart->rx_state!=UART_READY && timeout)
	  {
	    if((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
	    {
	    	timeout--;
	    }
	  }

	  if(!timeout)
		  return UART_TIMEOUT;
	  else
		  return UART_READY;
}
