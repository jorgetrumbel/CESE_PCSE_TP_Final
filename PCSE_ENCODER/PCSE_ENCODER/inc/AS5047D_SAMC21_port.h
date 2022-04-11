/***************************************************************************//**
 @file	   AS5047D_SAMC21_port.h
 @brief	   Header file for low-level driver for AS5047D encoder with SAMC21 microcontroller
 @author   AV
 ******************************************************************************/

#ifndef AS5047D_SAMC21_PORT_H_
#define AS5047D_SAMC21_PORT_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Volatile Registers
#define NOP				0x0000
#define ERRFL			0x0001
#define PROG			0x0003
#define DIAAGC			0x3FFC
#define CORDICMAG		0x3FFD
#define ANGLEUNC		0x3FFE
#define ANGLECOM		0x3FFF

// Non Volatile Registers
#define ZPOSM			0x0016
#define ZPOSL			0x0017
#define SETTINGS1		0x0018
#define SETTINGS2		0x0019

// Function responses
#define NO_ERR			0
#define ERR_SPI_FAILED	-1
#define ERR_ENC_COM		-2
#define ERR_DIAG_INIT	-3

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Write a register on the AS5047D memory
 * @param mem_address: address of the register
 * @param * data: pointer to data to write
 * @return errors
 */
int32_t AS5047D_Write(uint16_t mem_address, uint16_t * data);

/**
 * @brief Read a register from the AS5047D memory
 * @param mem_address: address of the register
 * @param * data: pointer to attach data from register
 * @return errors
 */
int32_t AS5047D_Read(uint16_t mem_address, uint16_t * data);


/*******************************************************************************
 ******************************************************************************/

#endif // AS5047D_SAMC21_PORT_H_
