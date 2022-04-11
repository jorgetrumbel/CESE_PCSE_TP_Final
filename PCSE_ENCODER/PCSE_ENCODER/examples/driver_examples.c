/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

/**
 * Example of using SPI_AS5047D to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_AS5047D[12] = "Hello World!";

void SPI_AS5047D_example(void)
{
	SPI_AS5047D_write_block((void *)example_SPI_AS5047D, 12);
}

/**
 * Example of using UART_TTL to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_UART_TTL[12] = "Hello World!";

static void tx_cb_UART_TTL(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void UART_TTL_example(void)
{
	struct io_descriptor *io;

	usart_async_register_callback(&UART_TTL, USART_ASYNC_TXC_CB, tx_cb_UART_TTL);
	/*usart_async_register_callback(&UART_TTL, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&UART_TTL, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&UART_TTL, &io);
	usart_async_enable(&UART_TTL);

	io_write(io, example_UART_TTL, 12);
}

void delay_example(void)
{
	delay_ms(5000);
}
