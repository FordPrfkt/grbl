/*!
***     \file	  TMC2660.c
***     \ingroup  TMC2660
***     \author   Daniel
***     \date	  1/14/2018 3:14:49 PM
***     \brief    TODO
***
******************************************************************************/

/* Header file guard symbol to prevent multiple includes */
#ifndef TMC2660_H_
#define TMC2660_H_

/* storage class specifier if used with C++ */
#ifdef  __cplusplus
  extern "C" {
#endif

/*=============================================================================
=======                            INCLUDES                             =======
=============================================================================*/

/*=============================================================================
=======               DEFINES & MACROS FOR GENERAL PURPOSE              =======
=============================================================================*/

/*=============================================================================
=======                       CONSTANTS  &  TYPES                       =======
=============================================================================*/
typedef enum
{
	TMC_X_AXIS = 0,
	TMC_Y_AXIS,
	TMC_Z_AXIS
}TMC_Axis_t;

typedef struct {
	uint16_t sgCurrent:5;
	uint16_t csCurrent:5;
	uint16_t standStill:1;
	uint16_t openLoadA:1;
	uint16_t openLoadB:1;
	uint16_t shortA:1;
	uint16_t shortB:1;
	uint16_t overTempWarn:1;
	uint8_t overTempShtdn:1;
	uint8_t stallGuard:1;
	uint8_t reserved:6;
}TMC_Status_t;

/*=============================================================================
=======                              EXPORTS                            =======
=============================================================================*/
void TMC_Init(void);
void TMC_GetStatus(TMC_Axis_t axis, TMC_Status_t *status);
void TMC_Enable(void);
void TMC_Disable(void);

/* end of storage class specifier if used with C++ */
#ifdef  __cplusplus
}
#endif

#endif /*TMC2660_H_*/
