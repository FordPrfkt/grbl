/*!
***     \file	  TMC2660.c
***     \ingroup  TMC2660
***     \author   Daniel
***     \date	  1/14/2018 3:15:00 PM
***     \brief    TODO
***
******************************************************************************/

/*=============================================================================
=======                            INCLUDES                             =======
=============================================================================*/
#include "grbl.h"
#include "TMC2660.h"
/*=============================================================================
=======               DEFINES & MACROS FOR GENERAL PURPOSE              =======
=============================================================================*/
#define ADDRESS2 19
#define ADDRESS1 18
#define ADDRESS0 17

#define DRVCTRL_ADDR 0
#define INTPOL 9
#define DEDGE 8
#define MRES3 3
#define MRES2 2
#define MRES1 1
#define MRES0 0

#define CHOPCONF_ADDR 4
#define TBL1 16
#define TBL0 15
#define CHM 14
#define RNDTF 13
#define HDEC1 12
#define HDEC0 11
#define HEND3 10
#define HEND2 9
#define HEND1 8
#define HEND0 7
#define HSTRT2 6
#define HSTRT1 5
#define HSTRT0 4
#define TOFF3 3
#define TOFF2 2
#define TOFF1 1
#define TOFF0 0

#define SMARTEN_ADDR 5
#define SEIMIN 15
#define SEDN1 14
#define SEDN0 13
#define SEMAX3 11
#define SEMAX2 10
#define SEMAX1 9
#define SEMAX0 8
#define SEUP1 6
#define SEUP0 5
#define SEMIN3 3
#define SEMIN2 2
#define SEMIN1 1
#define SEMIN0 0

#define SGCSCONF_ADDR 6
#define SFILT 16
#define SGT6 14
#define SGT5 13
#define SGT4 12
#define SGT3 11
#define SGT2 10
#define SGT1 9
#define SGT0 8
#define CS4 4
#define CS3 3
#define CS2 2
#define CS1 1
#define CS0 0

#define DRVCONF_ADDR 7
#define SLPH1 15
#define SLPH0 14
#define SLPL1 13
#define SLPL0 12
#define DISS2G 10
#define TS2G1 9
#define TS2G0 8
#define SDOFF 7
#define VSENSE 6
#define RDSEL1 5
#define RDSEL0 4

#define TMC_WRITE_CONF(_a, _b, _c) (_a[_b/8] = (_c << _b))
#define TMC_GET_DATA(_a, _b) (_a[_b/8] >> _b)

/*=============================================================================
=======                       CONSTANTS  &  TYPES                       =======
=============================================================================*/

/*=============================================================================
=======                VARIABLES & MESSAGES & RESSOURCEN                =======
=============================================================================*/
#ifdef USE_TMC2660
	uint8_t drvCtrl[3] = {0,0,1};
	uint8_t chopConf[3]= {0x08,0x63,0x3A};
	uint8_t smartEn[3]= {0x0A,0x04,0x60};
	uint8_t sgcsConf[3]= {0x0D,0x3F,0x1F};
	uint8_t drvConf[3]= {0x0E,0xA4,0x20};

	uint32_t inBuf;

	#ifndef USE_SPI
	#error "The TMC Driver needs the SPI interface to be enabled";
	#endif
#endif // USE_TMC2660

/*=============================================================================
=======                              METHODS                           =======
=============================================================================*/
void tmc_SendData(TMC_Axis_t axis, uint8_t data[]);
void tmc_ReceiveData(TMC_Axis_t axis, uint8_t dataOut[], uint8_t dataIn[]);

/* -----------------------------------------------------
* --               Public functions                  --
* ----------------------------------------------------- */
void TMC_Init(void)
{
#ifdef USE_TMC2660
    /* Init Port & Disable Drivers */
	TMC_ENABLE_DDR |= (1 << TMC_ENABLE_BIT);
	TMC_ENABLE_PORT |= (1 << TMC_ENABLE_BIT);

    /* Init Port & Disable Chip select */
	TMC_CSX_DDR |= (1 << TMC_CSX_BIT);
	TMC_CSY_DDR |= (1 << TMC_CSY_BIT);
	TMC_CSZ_DDR |= (1 << TMC_CSZ_BIT);	
	TMC_CSX_PORT |= (1 << TMC_CSX_BIT);
	TMC_CSY_PORT |= (1 << TMC_CSY_BIT);
	TMC_CSZ_PORT |= (1 << TMC_CSZ_BIT);
	
/*	drvCtrl = 0x6;
	chopConf = 0x8633A;
	smartEn = 0xA0460;
	sgcsConf = 0xD3F1F;
	drvConf = 0xEA420;*/

	tmc_SendData(TMC_X_AXIS, drvCtrl);
	tmc_SendData(TMC_X_AXIS, chopConf);
	tmc_SendData(TMC_X_AXIS, smartEn);
	tmc_SendData(TMC_X_AXIS, sgcsConf);
	tmc_SendData(TMC_X_AXIS, drvConf);

	tmc_SendData(TMC_Y_AXIS, drvCtrl);
	tmc_SendData(TMC_Y_AXIS, chopConf);
	tmc_SendData(TMC_Y_AXIS, smartEn);
	tmc_SendData(TMC_Y_AXIS, sgcsConf);
	tmc_SendData(TMC_Y_AXIS, drvConf);

	tmc_SendData(TMC_Z_AXIS, drvCtrl);
	tmc_SendData(TMC_Z_AXIS, chopConf);
	tmc_SendData(TMC_Z_AXIS, smartEn);
	tmc_SendData(TMC_Z_AXIS, sgcsConf);
	tmc_SendData(TMC_Z_AXIS, drvConf);

	tmc_ReceiveData(TMC_X_AXIS, drvConf, (uint8_t*)&inBuf);

#endif // USE_TMC2660
}

void TMC_GetStatus(TMC_Axis_t axis, TMC_Status_t *status)
{
#ifdef USE_TMC2660
	tmc_ReceiveData(axis, (uint8_t*)&drvConf, (uint8_t*)&inBuf);
#endif // USE_TMC2660
}

void TMC_Enable(void)
{
	TMC_ENABLE_PORT &= ~(1 << TMC_ENABLE_BIT);
}

void TMC_Disable(void)
{
	TMC_ENABLE_PORT |= (1 << TMC_ENABLE_BIT);
}

/* -----------------------------------------------------
* --               Private functions                  --
* ----------------------------------------------------- */
void tmc_SendData(TMC_Axis_t axis, uint8_t data[])
{
	switch (axis)
	{
		case TMC_X_AXIS:
			TMC_CSX_PORT &= ~(1 << TMC_CSX_BIT);
			SPI_Transmit(data, 3);
			TMC_CSX_PORT |= (1 << TMC_CSX_BIT);
		break;

		case TMC_Y_AXIS:
			TMC_CSY_PORT &= ~(1 << TMC_CSY_BIT);
			SPI_Transmit(data, 3);
			TMC_CSY_PORT |= (1 << TMC_CSY_BIT);
		break;

		case TMC_Z_AXIS:
			TMC_CSZ_PORT &= ~(1 << TMC_CSZ_BIT);
			SPI_Transmit(data, 3);
			TMC_CSZ_PORT |= (1 << TMC_CSZ_BIT);
		break;

		default:
		break;
	}
}

void tmc_ReceiveData(TMC_Axis_t axis, uint8_t dataOut[], uint8_t dataIn[])
{
	switch (axis)
	{
		case TMC_X_AXIS:
		TMC_CSX_PORT &= ~(1 << TMC_CSX_BIT);
		SPI_TransmitReceive(dataOut, dataIn, 3);
		TMC_CSX_PORT |= (1 << TMC_CSX_BIT);
		break;

		case TMC_Y_AXIS:
		TMC_CSY_PORT &= ~(1 << TMC_CSY_BIT);
		SPI_TransmitReceive(dataOut, dataIn, 3);
		TMC_CSY_PORT |= (1 << TMC_CSY_BIT);
		break;

		case TMC_Z_AXIS:
		TMC_CSZ_PORT &= ~(1 << TMC_CSZ_BIT);
		SPI_TransmitReceive(dataOut, dataIn, 3);
		TMC_CSZ_PORT |= (1 << TMC_CSZ_BIT);
		break;

		default:
		break;
	}
}