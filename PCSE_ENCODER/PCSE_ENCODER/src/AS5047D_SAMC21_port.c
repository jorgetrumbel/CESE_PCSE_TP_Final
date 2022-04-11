/***************************************************************************//**
 @file     AS5047D_SAMC21_port.c
 @brief	   Source file for low-level driver for AS5047D encoder with SAMC21 microcontroller
 @author   AV
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <inc/AS5047D_SAMC21_port.h>
#include <atmel_start_pins.h>
#include <hal_gpio.h>
#include <spi_lite.h>
#include <hal_delay.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Pins
#define ENCODER_SELECT_PIN SPI_nCS_PIN

// SPI
#define SPI_N_BYTES		2
#define LB				1
#define HB				0
#define LB_SELECT		0x00FF
#define HB_SELECT		0x3F00

// Bits
#define WR_BIT			6
#define RD_BIT			6
#define ERR_BIT			6
#define PAR_BIT			7

#define BITS_IN_BYTE    8
#define BYTES_IN_INT    2
#define BYTES_IN_CHAR   1
#define BITS_IN_INT     BITS_IN_BYTE*BYTES_IN_INT

// Macros
#define CHIP_SELECT		gpio_set_pin_level(ENCODER_SELECT_PIN, false)
#define CHIP_DEELECT	gpio_set_pin_level(ENCODER_SELECT_PIN, true)

//Utility Macros
#ifndef BITSET
#define BITSET(d,b) ((d) |= 1U << (b))
#define BITCLR(d,b) ((d) &= ~(1U << (b)))
#define BITTGL(d,b) ((d) ^= 1U << (b))
#define BITCHK(d,b) (0 != ((d) & (1U << (b))))
#define BOOLEAN(v)  (0 != (v))
#endif // BITSET

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
 * @brief Check parity of a integer
 * @param input: Integer
 * @return parity
 */
uint32_t parityCheck(uint32_t input);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static uint8_t rx_array[SPI_N_BYTES];
static uint8_t tx_array[SPI_N_BYTES];

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int32_t AS5047D_Write(uint16_t mem_address, uint16_t * data){
	
	// SEND ADDRESS
	// Low level at CS of encoder
	CHIP_SELECT;
	
	// Prepare address
	tx_array[HB] = (uint8_t) (mem_address >> BITS_IN_BYTE);
	tx_array[LB] = (uint8_t) (mem_address & LB_SELECT);
	
	// Clear write bit and check parity
	BITCLR(tx_array[HB], WR_BIT);
	if ( parityCheck(tx_array[HB]) ^ parityCheck(tx_array[LB])) BITSET(tx_array[HB], PAR_BIT);
	
	// Transfer address
	rx_array[HB] = SPI_AS5047D_exchange_data(tx_array[HB]);
	rx_array[LB] = SPI_AS5047D_exchange_data(tx_array[LB]);
	
	// High level at CS of encoder and 350 ns delay (1000ns)
	CHIP_DEELECT;
	delay_us(1); //TODO: check if it can be removed
	
	// SEND DATA
	// Low level at CS of encoder
	CHIP_SELECT;
	
	// Prepare value
	tx_array[HB] = (uint8_t) (*data >> BITS_IN_BYTE);
	tx_array[LB] = (uint8_t) (*data & 0x00FF);
	
	// Clear write bit and check parity
	BITCLR(tx_array[HB], WR_BIT);
	if ( parityCheck(tx_array[HB]) ^ parityCheck(tx_array[LB])) BITSET(tx_array[HB], PAR_BIT);
	
	// Transfer first command
	rx_array[HB] = SPI_AS5047D_exchange_data(tx_array[HB]);
	rx_array[LB] = SPI_AS5047D_exchange_data(tx_array[LB]);
	
	// High level at CS of encoder
	CHIP_DEELECT;
	delay_us(1); //TODO: check if it can be removed
	if ( BITCHK(rx_array[HB], ERR_BIT) )
	return ERR_ENC_COM; // Encoder command error
	
	// SEND NO OPERATION
	// Low level at CS of encoder
	CHIP_SELECT;
	
	// Prepare value
	tx_array[HB] = (uint8_t) NOP;
	tx_array[LB] = (uint8_t) NOP;
	
	// Transfer first command
	rx_array[HB] = SPI_AS5047D_exchange_data(tx_array[HB]);
	rx_array[LB] = SPI_AS5047D_exchange_data(tx_array[LB]);
	
	// High level at CS of encoder
	CHIP_DEELECT;
	if ( BITCHK(rx_array[HB], ERR_BIT) )
	return ERR_ENC_COM; // Encoder command error
	
	return NO_ERR; // Success
}

int32_t AS5047D_Read(uint16_t mem_address, uint16_t * data){

	// SEND ADDRESS
	// Low level at CS of encoder
	CHIP_SELECT;
	
	// Prepare address
	tx_array[HB] = (uint8_t) (mem_address >> BITS_IN_BYTE);
	tx_array[LB] = (uint8_t) (mem_address & LB_SELECT);
	
	// Set read bit and check parity
	BITSET(tx_array[HB], RD_BIT);
	if ( parityCheck(tx_array[HB]) ^ parityCheck(tx_array[LB])) BITSET(tx_array[HB], PAR_BIT);
	
	// Transfer address
	rx_array[HB] = SPI_AS5047D_exchange_data(tx_array[HB]);
	rx_array[LB] = SPI_AS5047D_exchange_data(tx_array[LB]);
	
	// High level at CS of encoder and 350 ns delay (1000ns)
	CHIP_DEELECT;
	delay_us(1); //TODO: check if it can be removed
	
	// SEND NO OPERATION AND GET DATA
	// Low level at CS of encoder
	CHIP_SELECT;
	
	// Prepare value
	tx_array[HB] = (uint8_t) NOP;
	tx_array[LB] = (uint8_t) NOP;
	
	// Set read bit and check parity
	BITSET(tx_array[HB], RD_BIT);
	if ( parityCheck(tx_array[HB]) ^ parityCheck(tx_array[LB])) BITSET(tx_array[HB], PAR_BIT);
	
	// Transfer first command
	rx_array[HB] = SPI_AS5047D_exchange_data(tx_array[HB]);
	rx_array[LB] = SPI_AS5047D_exchange_data(tx_array[LB]);
	
	// High level at CS of encoder
	CHIP_DEELECT;
	
	if ( BITCHK(rx_array[HB], ERR_BIT) ) return ERR_ENC_COM; // Encoder command error
	//ESTE BITCHECK POR ALGUNA RAZON SIEMPRE PASA, SEGUN EL DATASHEET SIGNIFICA QUYE HAY UN ERROR EN EL MENSAJE,
	//SIN EMBARGO EL ENCODER PARECE MANDAR CORRECTAMENTE LA POSICION DEL ENCODER, HAY QUE ASEGURARSE QUE ESTO ESTE BIEN
	
	// Attach data
	*data = (uint16_t) (rx_array[HB] << BITS_IN_BYTE);
	*data += (uint16_t) rx_array[LB];
	
	return NO_ERR; // Success
}
/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

uint32_t parityCheck(uint32_t input){
	uint32_t parity = 0;
	while (input > 0)
	{
		parity = (parity + (input & 0x01)) % 2;
		input >>= 1;
	}
	return (parity);
}

/******************************************************************************/
