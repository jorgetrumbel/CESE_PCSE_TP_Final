/***************************************************************************//**
 @file	   AS5047D.h
 @brief	   Header file for high-level driver for AS5047D encoder
 @author   AV
 ******************************************************************************/

#ifndef AS5047D_H_
#define AS5047D_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define AS5047D_MAX_SPEED	242		// [Hz]
#define AS5047D_RANGE		16384	// steps

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

// Diag Error report
typedef enum{
	AS5047D_NO_DIAG_ERR = 0,
	AS5047D_SOURCE,
	AS5047D_ERR_MAGL,
	AS5047D_ERR_MAGH,
	AS5047D_ERR_COF,
	AS5047D_ERR_LF} AS5047D_diag_error_source_e;

typedef enum{
	AS5047D_NO_COMM_ERR = 0,
	AS5047D_PARITY_ERROR,
	AS5047D_INVALID_COMM_ERROR,
	AS5047D_FRAMING_ERROR} AS5047D_comm_error_e;
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize AS5047D
 * @return errors
 */
int32_t AS5047DInit(void);

/**
 * @brief Check AS5047D diagnostics
 * @return errors
 */
AS5047D_diag_error_source_e AS5047D_CheckDiagnostics(void);

/**
 * @brief Check AS5047D diagnostics
 * @return errors
 */
AS5047D_comm_error_e AS5047D_CheckCommErrors(void);

/**
 * @brief Automatic Gain Control from AS5047D
 * @param agc: pointer to agc
 * @return errors
 */
int32_t AS5047D_getAGC(uint8_t * agc);

/**
 * @brief CORDIC magnitude from AS5047D
 * @param cmag: pointer to cmag
 * @return errors
 */
int32_t AS5047D_getCMAG(uint16_t * cmag);

/**
 * @brief Position from AS5047D
 * @param position: pointer to position
 * @return errors
 */
int32_t AS5047D_getPosition(int16_t * position);

/**
 * @brief Raw position from AS5047D
 * @param position: pointer to position
 * @return errors
 */
int32_t AS5047D_getRawPosition(int16_t * position);

/**
 * @brief Transforms encoder position to angle
 * @param position: pointer to position
 * @return angle
 */
uint16_t AS5047D_transformPositionToAngle(int16_t * position);
/*******************************************************************************
 ******************************************************************************/

#endif // AS5047D_H_
