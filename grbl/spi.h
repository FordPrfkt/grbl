/*!
***     \file	  spi.c
***     \ingroup  spi
***     \author   Daniel
***     \date	  1/13/2018 5:23:43 PM
***     \brief    TODO
***
******************************************************************************/

/* Header file guard symbol to prevent multiple includes */
#ifndef spi_H_
#define spi_H_

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
typedef enum {
	SPI_OK,
	SPI_COLLISION,
	SPI_FAIL
}SPI_Result_t;

typedef enum
{
	SPI_MSB_FIRST = 0,
	SPI_LSB_FIRST = 1
} SPI_DataOrder_t;

typedef enum
{
	SPI_CP_RISING  = 0,
	SPI_CP_FALLING = 1
} SPI_ClkPol_t;

typedef enum
{
	SPI_CLK_LEADING = 0,
	SPI_CLK_TRAILING = 1
} SPI_ClkPhs_t;

typedef enum
{
	SPI_DIV_4 = 0,
	SPI_DIV_16 = 1,
	SPI_DIV_64 = 2,
	SPI_DIV_128 = 3,
	SPI_DIV_2 = 4,
	SPI_DIV_8 = 5,
	SPI_DIV_32 = 6,
	SPI_DIV_64_2 = 7,
} SPI_Prescaler_t;

/*=============================================================================
=======                              EXPORTS                            =======
=============================================================================*/
void SPI_Init(SPI_DataOrder_t dataOrder, SPI_ClkPol_t clockPolarity, SPI_ClkPhs_t clockPhase, SPI_Prescaler_t clockPrescaler);
void SPI_SetMode(SPI_DataOrder_t dataOrder, SPI_ClkPol_t clockPolarity, SPI_ClkPhs_t clockPhase, SPI_Prescaler_t clockPrescaler);
SPI_Result_t SPI_Transmit(uint8_t data[], uint16_t length);
SPI_Result_t SPI_Receive(uint8_t data[], uint16_t length);
SPI_Result_t SPI_TransmitReceive(uint8_t dataOut[], uint8_t dataIn[], uint16_t length);

/* end of storage class specifier if used with C++ */
#ifdef  __cplusplus
}
#endif

#endif /*spi_H_*/
