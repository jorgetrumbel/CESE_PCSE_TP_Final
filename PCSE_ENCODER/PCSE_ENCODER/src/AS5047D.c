/***************************************************************************//**
 @file     AS5047D.c
 @brief	   Source file for high-level driver for AS5047D encoder
 @author   AV
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <inc/AS5047D.h>
#include <inc/AS5047D_SAMC21_port.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Data
#define AS5047D_DATA		0x3FFF
#define AS5047D_AGC_DATA	0x00FF

// DIAAGN bits
#define MAGL 0x0800
#define MAGH 0x0400
#define COF  0x0200
#define LF   0x0100

// ERRFL bits
#define PARERR	0x0002
#define INVCOMM 0x0001
#define FRERR	0x0000


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int32_t AS5047DInit(void){
	uint16_t aux_int; 
	AS5047D_Read(ERRFL, &aux_int); //Clear errors
	return NO_ERR;
}

AS5047D_diag_error_source_e AS5047D_CheckDiagnostics(void){
	//Create return value
	AS5047D_diag_error_source_e ret_value = AS5047D_NO_DIAG_ERR; 
	
	// Diagnostic register
	uint16_t diagnostic_reg = 0;
	
	// Read the register
	AS5047D_Read(DIAAGC, &diagnostic_reg);
	
	// Evaluate response
	if (diagnostic_reg & MAGL){
		ret_value = AS5047D_ERR_MAGL;
	}  
	else if (diagnostic_reg & MAGH){
		ret_value = AS5047D_ERR_MAGH;
	}
	else if (diagnostic_reg & COF){
		ret_value = AS5047D_ERR_COF;
	}
	else if (!(diagnostic_reg & LF)){
		ret_value = AS5047D_ERR_LF;
	}
	
	return ret_value;
}

AS5047D_comm_error_e AS5047D_CheckCommErrors(void){
	//Create return value
	AS5047D_comm_error_e ret_value = AS5047D_NO_COMM_ERR;
	
	// Error register
	uint16_t error_reg = 0;
	
	// Read the register
	AS5047D_Read(ERRFL, &error_reg);
	
	// Evaluate response
	if (error_reg & PARERR){
		ret_value = AS5047D_PARITY_ERROR;
	}
	else if (error_reg & INVCOMM){
		ret_value = AS5047D_INVALID_COMM_ERROR;
	}
	else if (error_reg & FRERR){
		ret_value = AS5047D_FRAMING_ERROR;
	}
	
	return ret_value;
}

int32_t AS5047D_getAGC(uint8_t * agc){
	// Diagnostic register
	uint16_t diagnostic_reg = 0;
	
	// Read the register
	int32_t response = AS5047D_Read(DIAAGC, &diagnostic_reg);
	
	* agc = (uint8_t) (diagnostic_reg & AS5047D_AGC_DATA);
	
	return response;
}

int32_t AS5047D_getCMAG(uint16_t * cmag){
	uint16_t data = 0;
	int32_t response = AS5047D_Read(CORDICMAG, &data);
	
	* cmag = (uint16_t) (data & AS5047D_DATA);
	
	return response;
}

int32_t AS5047D_getPosition(int16_t * position){
	uint16_t data = 0;
	int32_t response = AS5047D_Read(ANGLECOM, &data);
	
	* position = (int16_t) (data & AS5047D_DATA);
	
	return response;
}

int32_t AS5047D_getRawPosition(int16_t * position){
	uint16_t data = 0;
	int32_t response = AS5047D_Read(ANGLEUNC, &data);
	
	
	* position = (int16_t) (data & AS5047D_DATA);
	
	return response;
}

uint16_t AS5047D_transformPositionToAngle(int16_t * position){
	uint16_t angle_val = 0;
	
	angle_val = ((*position * 3600) / AS5047D_RANGE) / 10;
	
	return angle_val;
}

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/******************************************************************************/
