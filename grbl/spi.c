/*!
***     \file	  spi.c
***     \ingroup  spi
***     \author   Daniel
***     \date	  1/13/2018 5:23:34 PM
***     \brief    TODO
***
******************************************************************************/

/*=============================================================================
 =======                            INCLUDES                             =======
 =============================================================================*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "spi.h"
/*=============================================================================
 =======               DEFINES & MACROS FOR GENERAL PURPOSE              =======
 =============================================================================*/

/*=============================================================================
 =======                       CONSTANTS  &  TYPES                       =======
 =============================================================================*/

/*=============================================================================
 =======                VARIABLES & MESSAGES & RESSOURCEN                =======
 =============================================================================*/
volatile uint8_t dummy;

/*=============================================================================
 =======                              METHODS                           =======
 =============================================================================*/
 void spi_sendByte(uint8_t data);
  uint8_t spi_receiveByte(void);
 
/* -----------------------------------------------------
 * --               Public functions                  --
 * ----------------------------------------------------- */
void SPI_Init(SPI_DataOrder_t dataOrder, SPI_ClkPol_t clockPolarity, SPI_ClkPhs_t clockPhase, SPI_Prescaler_t clockPrescaler)
{
	SPCR = 0;
	SPCR |= ((1 << SPE) | (1 << MSTR));
	SPI_SetMode(dataOrder, clockPolarity, clockPhase, clockPrescaler);
	
	dummy = SPSR;
	dummy = SPDR;
}

void SPI_SetMode(SPI_DataOrder_t dataOrder, SPI_ClkPol_t clockPolarity, SPI_ClkPhs_t clockPhase, SPI_Prescaler_t clockPrescaler)
{
	SPCR &= 0xD0; // Reset SPCR to Init State
	SPCR |= (dataOrder << DORD);
	SPCR |= (clockPolarity << CPOL);
	SPCR |= (clockPhase << CPHA);
	SPCR |= (0x03 & (clockPrescaler << SPR1));
	if ((clockPrescaler & 0x04) == 0x04)
	{
		SPSR |= (1 << SPI2X);
	}
}

SPI_Result_t SPI_Transmit(uint8_t data[], uint16_t length)
{
	uint16_t ctr = 0;
	SPI_Result_t result = SPI_OK;

	/* Reset SPIF & WCOL */
	dummy = SPSR;
	dummy = SPDR;

	do 
	{
		spi_sendByte(data[ctr]);
		ctr++;
		
		do 
		{
			if ((SPSR & (1 << WCOL)) == (1 << WCOL))
			{
				result = SPI_COLLISION;
				break;
			}
		} while ((SPSR & (1 << SPIF)) == 0);


		if (SPI_OK != result)
		{
			break;
		}
	} while (ctr < length);
	
	/* Reset SPIF & WCOL */
	dummy = SPSR;
	dummy = SPDR;

	return result;
}

SPI_Result_t SPI_Receive(uint8_t data[], uint16_t length)
{
	uint16_t ctr = 0;
	SPI_Result_t result = SPI_OK;

	/* Reset SPIF & WCOL */
	dummy = SPSR;
	dummy = SPDR;

	do
	{
		spi_sendByte(0);
		
		do
		{
			if ((SPSR & (1 << WCOL)) == (1 << WCOL))
			{
				result = SPI_COLLISION;
				break;
			}
		} while ((SPSR & (1 << SPIF)) == 0);
		
		if (SPI_OK != result)
		{
			break;
		}

		data[ctr] = spi_receiveByte();
		ctr++;
	} while (ctr < length);
	
	/* Reset SPIF & WCOL */
	dummy = SPSR;
	dummy = SPDR;

	return result;
}

SPI_Result_t SPI_TransmitReceive(uint8_t dataOut[], uint8_t dataIn[], uint16_t length)
{
	uint16_t ctr = 0;
	SPI_Result_t result = SPI_OK;

	/* Reset SPIF & WCOL */
	dummy = SPSR;
	dummy = SPDR;

	do
	{
		spi_sendByte(dataOut[ctr]);
		
		do
		{
			if ((SPSR & (1 << WCOL)) == (1 << WCOL))
			{
				result = SPI_COLLISION;
				break;
			}
		} while ((SPSR & (1 << SPIF)) == 0);
		
		if (SPI_OK != result)
		{
			break;
		}

		dataIn[ctr] = spi_receiveByte();
		ctr++;
	} while (ctr < length);
	
	/* Reset SPIF & WCOL */
	dummy = SPSR;
	dummy = SPDR;

	return result;
}

/* -----------------------------------------------------
 * --               Private functions                  --
 * ----------------------------------------------------- */
 void spi_sendByte(uint8_t data)
 {
	SPDR = data;
 }

 uint8_t spi_receiveByte(void)
 {
	return SPDR;
 }
