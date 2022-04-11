#include <atmel_start.h>
#include "uart_ttl_handler.h"
#include <inc/AS5047D.h>
#include <stdio.h>
#include "hal_delay.h"

//app prototypes
void app_diagnostics_parse(void);
void app_comm_parse(void);

//UART variables
static uint8_t uart_ttl_send_data[100];

//Program variables
static int16_t current_encoder_value;
static int16_t current_encoder_raw_value;
static uint8_t agc;
static uint16_t cmag;
static uint8_t app_counter;
static uint16_t angle;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	//init uart interface for debugging
	uart_ttl_init();
	
	//init AS5047D driver
	AS5047DInit();
	
	/* Replace with your application code */
	while (1) {
		AS5047D_getPosition(&current_encoder_value);
		sprintf((char*)uart_ttl_send_data, "\nEncoder Pos: %05d", current_encoder_value);
		uart_ttl_send(uart_ttl_send_data, 20);
		delay_ms(100);
		
		AS5047D_getRawPosition(&current_encoder_raw_value);
		sprintf((char*)uart_ttl_send_data, "\nEncoder RAW Pos: %05d", current_encoder_raw_value);
		uart_ttl_send(uart_ttl_send_data, 26);
		delay_ms(100);
		
		angle = AS5047D_transformPositionToAngle(&current_encoder_value);
		sprintf((char*)uart_ttl_send_data, "\nEncoder Angle: %03u\n\n", angle);
		uart_ttl_send(uart_ttl_send_data, 22);
		delay_ms(100);
		
		delay_ms(1000);
		app_counter++;
		
		if(app_counter >= 10){
			app_counter = 0;
			
			sprintf((char*)uart_ttl_send_data, "\n\n*****STATUS*****\n");
			uart_ttl_send(uart_ttl_send_data, 20);
			delay_ms(100);
			
			app_diagnostics_parse();
			
			app_comm_parse();
			
			AS5047D_getCMAG(&cmag);
			sprintf((char*)uart_ttl_send_data, "\nCORDIC MAG: %05d", cmag);
			uart_ttl_send(uart_ttl_send_data, 19);
			delay_ms(100);
			
			AS5047D_getAGC(&agc);
			sprintf((char*)uart_ttl_send_data, "\nGAIN: %03d", agc);
			uart_ttl_send(uart_ttl_send_data, 11);
			delay_ms(100);
			
			sprintf((char*)uart_ttl_send_data, "\n***END STATUS***\n\n");
			uart_ttl_send(uart_ttl_send_data, 20);
			delay_ms(100);
		}
	}
}

void app_diagnostics_parse(void){
	AS5047D_diag_error_source_e diag_error;
	diag_error = AS5047D_CheckDiagnostics();
	switch(diag_error){
		case AS5047D_NO_DIAG_ERR:{
			sprintf((char*)uart_ttl_send_data, "\nDiagnostics: OK");
			break;	
		}
		case AS5047D_ERR_MAGL:{
			sprintf((char*)uart_ttl_send_data, "\nDiagnostics: MAGL");
			break;	
		}
		case AS5047D_ERR_MAGH:{
			sprintf((char*)uart_ttl_send_data, "\nDiagnostics: MAGH");
			break;	
		}
		case AS5047D_ERR_COF:{
			sprintf((char*)uart_ttl_send_data, "\nDiagnostics: COF");
			break;	
		}
		case AS5047D_ERR_LF:{
			sprintf((char*)uart_ttl_send_data, "\nDiagnostics: LF");
			break;
		}	
		default:{
			break;
		}
	}
	uart_ttl_send(uart_ttl_send_data, 20);
	delay_ms(100);
}

void app_comm_parse(void){
	AS5047D_comm_error_e comm_error;
	comm_error = AS5047D_CheckCommErrors();
	switch(comm_error){
		case AS5047D_NO_COMM_ERR:{
			sprintf((char*)uart_ttl_send_data, "\nComms: OK");
			break;	
		}
		case AS5047D_PARITY_ERROR:{
			sprintf((char*)uart_ttl_send_data, "\nComms: PAR");
			break;	
		}
		case AS5047D_INVALID_COMM_ERROR:{
			sprintf((char*)uart_ttl_send_data, "\nComms: INV");
			break;	
		}
		case AS5047D_FRAMING_ERROR:{
			sprintf((char*)uart_ttl_send_data, "\nComms: FRA");
			break;	
		}
		default:{
			break;
		}
	}
	uart_ttl_send(uart_ttl_send_data, 12);
	delay_ms(100);
}