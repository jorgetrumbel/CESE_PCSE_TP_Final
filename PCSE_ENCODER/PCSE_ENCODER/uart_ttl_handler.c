/***************************************************************************//**
 @file     
 @brief	   
 @author   AV
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <atmel_start.h>
#include "driver_init.h"
#include "utils.h"
#include <stdbool.h>
#include <string.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define UART_TTL_MAX_MESSAGE_LENGTH 100

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief
 * @param
 * @return
 */
static void tx_cb_UART(const struct usart_async_descriptor *const io_descr);

/**
 * @brief
 * @param
 * @return
 */
static void rx_cb_UART(const struct usart_async_descriptor *const io_descr);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

//Test messages
static uint8_t UART_message[12] = "\nReceived\n";
static uint8_t UART_message_sent[] = "\nSent\n";

static uint8_t UART_message_rec[UART_TTL_MAX_MESSAGE_LENGTH];
static bool msg_sent, msg_rec;

static struct io_descriptor *io;

static struct usart_async_status uart_status;

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void uart_ttl_init(void)
{
	usart_async_register_callback(&UART_TTL, USART_ASYNC_TXC_CB, tx_cb_UART);
	usart_async_register_callback(&UART_TTL, USART_ASYNC_RXC_CB, rx_cb_UART);
	//usart_async_register_callback(&USART_0, USART_ASYNC_ERROR_CB, err_cb);
	usart_async_get_io_descriptor(&UART_TTL, &io);
	usart_async_enable(&UART_TTL);

	io_write(io, UART_message, 12);
	msg_sent = true;
}

void uart_ttl_send(uint8_t * uart_ttl_msg, uint8_t length){
	io_write(io, uart_ttl_msg, length);
	msg_sent = true;
}

void uart_ttl_receive(uint8_t * uart_ttl_msg, uint8_t length){
	if(length < UART_TTL_MAX_MESSAGE_LENGTH){
		memcpy(uart_ttl_msg, UART_message_rec, length);
		msg_rec = false;
	}
}

bool uart_ttl_check_rx_msg(void){
	return msg_rec;
}

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void tx_cb_UART(const struct usart_async_descriptor *const io_descr)
{
	if(msg_sent){
		//io_write(io, UART_message_sent, 7);
		msg_sent = false;
	}
}

static void rx_cb_UART(const struct usart_async_descriptor *const io_descr)
{
	usart_async_get_status(&UART_TTL, &uart_status);
	if(uart_status.rxcnt > 1){
		io_write(io, UART_message, 10);
		msg_sent = true;
		io_read(io, UART_message_rec, 100);
		msg_rec = true;
	}
}

/******************************************************************************/
