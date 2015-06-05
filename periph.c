//-----------------------------------------------------------------------------
//   File:      periph.c
//   Contents:  Hooks required to implement USB peripheral function.
//
// $Archive: /USB/Examples/FX2LP/hid_kb/periph.c $
// $Date: 3/23/05 2:58p $
// $Revision: 2 $
//
//
//-----------------------------------------------------------------------------
// Copyright 2003, Cypress Semiconductor Corporation
//
// This software is owned by Cypress Semiconductor Corporation (Cypress) and is
// protected by United States copyright laws and international treaty provisions. Cypress
// hereby grants to Licensee a personal, non-exclusive, non-transferable license to copy,
// use, modify, create derivative works of, and compile the Cypress Source Code and
// derivative works for the sole purpose of creating custom software in support of Licensee
// product ("Licensee Product") to be used only in conjunction with a Cypress integrated
// circuit. Any reproduction, modification, translation, compilation, or representation of this
// software except as specified above is prohibited without the express written permission of
// Cypress.
//
// Disclaimer: Cypress makes no warranty of any kind, express or implied, with regard to
// this material, including, but not limited to, the implied warranties of merchantability and
// fitness for a particular purpose. Cypress reserves the right to make changes without
// further notice to the materials described herein. Cypress does not assume any liability
// arising out of the application or use of any product or circuit described herein. Cypress?
// products described herein are not authorized for use as components in life-support
// devices.
//
// This software is protected by and subject to worldwide patent coverage, including U.S.
// and foreign patents. Use may be limited by and subject to the Cypress Software License
// Agreement.
//-----------------------------------------------------------------------------
#pragma NOIV               // Do not generate interrupt vectors

#include "fx2.h"
#include "fx2regs.h"
#include "syncdly.h"

extern BOOL   GotSUD;         // Received setup data flag
extern BOOL   Sleep;
extern BOOL   Rwuen;
extern BOOL   Selfpwr;

WORD	pHIDDscr;
WORD	pReportDscr;
WORD	pReportDscrEnd;
extern code HIDDscr;
extern code HIDReportDscr; 
extern code HIDReportDscrEnd;

BYTE xdata buttons;
BYTE xdata oldbuttons;
BYTE xdata leds;

BYTE   Configuration;      // Current configuration
BYTE   AlternateSetting;   // Alternate settings

#define BTN_ADDR		0x20
#define LED_ADDR		0x21

#define UART_TX	PD6

void UART_Init(void)
{
	OED = 0x40;
	UART_TX = 1;
}

void nop_loop_delay(BYTE count)
{
    while (count--);
}

void UART_Tick(void)
{
	// CPU Freq set to 48MHz, UART band rate sim to 115200
	nop_loop_delay(8);
}

void UART_SendByte(BYTE ch)
{
	BYTE i = 10;
	WORD pad;

	// BIT0: start bit as 0
	// BIT1~8: char
	// BIT9: end bit as 1
	pad = (ch << 1) | 0x200;

	while (i--) {
		UART_TX = (pad & 1) ? 1 : 0;
		UART_Tick();
		pad >>= 1;
	}
}

void UART_PrintString(BYTE *s)
{
	while (*s != 0) {
		UART_SendByte(*s);
		s++;
	}
}

void UART_PrintHex(BYTE bData)
{
	BYTE i, bHalf, bChar;

	for (i=0; i<2; i++) {
		if (i == 0)
			bHalf = bData >> 4;
		else
			bHalf = bData & 0xF;

		if (bHalf < 10)
			bChar = '0' + bHalf;
		else
			bChar = 'A' - 10 + bHalf;
		UART_SendByte(bChar);
	}
}

void UART_PrintData(BYTE *pData, BYTE count)
{
	while (count--) {
		UART_PrintHex(*pData++);
		UART_SendByte(' ');
	}
}

//-----------------------------------------------------------------------------
// Task Dispatcher hooks
//   The following hooks are called by the task dispatcher.
//-----------------------------------------------------------------------------

void TD_Init(void)             // Called once at startup
{
	BYTE tmpbuf[] = {0x23, 0xf4, 0x99, 0x1e};

   CPUCS = ((CPUCS & ~bmCLKSPD) | bmCLKSPD1) ;
   
   leds = 0xFF;
   oldbuttons = 0xFF;

    OEA = 0x0f;
    IOA = 0x0F;

   EP1OUTCFG = 0xB0;       // valid, interrupt
   EP1INCFG = 0XB0;        // valid, interrupt

   EP2CFG = EP4CFG = EP6CFG = EP8CFG = 0;   // disable unused endpoints

   UART_Init();

#if 0
   while (1)
   	{
   		UART_PrintString("=== HELLO WORLD====: ");
		UART_PrintHex(IOA);
		UART_PrintString("\r\n");

		UART_PrintString("DUMP DATA: ");
		UART_PrintData(tmpbuf, 4);
		UART_PrintString("\r\n");

		{
		WORD j, i = 1000;
		IOA = ! (IOA & 1);
		for (i=0; i<1152; i++)
			for (j=0; j<100; j++)
			//EZUSB_Delay1ms();
			UART_Tick();
		}
   	}

	while (0) {
		WORD j, i = 1000;
		IOA = ! (IOA & 1);
		for (i=0; i<1152; i++)
			for (j=0; j<100; j++)
			//EZUSB_Delay1ms();
			UART_Tick();
	}
#endif
//	EZUSB_InitI2C();			// Initialize EZ-USB I2C controller


}

void TD_Poll(void)             // Called repeatedly while the device is idle
{
   static unsigned char keys, oldkeys;
   static unsigned char mled;

	if( !(EP1OUTCS & bmEPBUSY) )	// Is there something available
	{
		mled = IOA & 0X7;
		//leds = 0xFF;			//Turn all off

		//if (EP1OUTBUF[0] & 0x02)	//Caps
		//	IOB &= 0xBF;
		//if (EP1OUTBUF[0] & 0x01)	//Scroll
		//	IOB &= 0xF7;
		//if (EP1OUTBUF[0] & 0x04)	//Num
		//	IOB &= 0xFE;
		
		//if (mled & 0x1)
		//	IOA = 0;
		//else
		//	IOA = 1;

		//mled = EP1OUTBUF[0];
		//IOA = mled;
		//if (mled & 1)
		//	mled = mled & (~1);
		//else
		//	mled = mled | (1);

	    //if (EP1OUTBUF[0] & 1)
		//	mled = mled | (2);
		//else
		//	mled = mled & (~2);

		//IOA = mled;
		IOA = EP1OUTBUF[0];

		UART_PrintString("EP1OUTBUF: ");
		UART_PrintData(EP1OUTBUF, EP1OUTBC);
		UART_PrintString("\r\n");

		EP1OUTBC = 0;				//Rearm endpoint buffer
	}
}


BOOL TD_Suspend(void)          // Called before the device goes into suspend mode
{
   return(TRUE);
}

BOOL TD_Resume(void)          // Called after the device resumes
{
   return(TRUE);
}

//-----------------------------------------------------------------------------
// Device Request hooks
//   The following hooks are called by the end point 0 device request parser.
//-----------------------------------------------------------------------------

BOOL DR_GetDescriptor(void)
{
	BYTE length,i;

	pHIDDscr = (WORD)&HIDDscr;
	pReportDscr = (WORD)&HIDReportDscr;
	pReportDscrEnd = (WORD)&HIDReportDscrEnd;

	switch (SETUPDAT[3])
	{
		case GD_HID:					//HID Descriptor
			SUDPTRH = MSB(pHIDDscr);
			SUDPTRL = LSB(pHIDDscr);
			return (FALSE);
		case GD_REPORT:					//Report Descriptor
			length = pReportDscrEnd - pReportDscr;

         AUTOPTR1H = MSB(pReportDscr);
         AUTOPTR1L = LSB(pReportDscr);

         for(i=0;i<length;i++)
            EP0BUF[i]=XAUTODAT1;

         EP0BCL = length;
         return (FALSE);
		default:
			return(TRUE);
	}
}

BOOL DR_SetConfiguration(void)   // Called when a Set Configuration command is received
{
   Configuration = SETUPDAT[2];
   return(TRUE);            // Handled by user code
}

BOOL DR_GetConfiguration(void)   // Called when a Get Configuration command is received
{
   EP0BUF[0] = Configuration;
   EP0BCH = 0;
   EP0BCL = 1;
   return(TRUE);            // Handled by user code
}

BOOL DR_SetInterface(void)       // Called when a Set Interface command is received
{
   AlternateSetting = SETUPDAT[2];
   return(TRUE);            // Handled by user code
}

BOOL DR_GetInterface(void)       // Called when a Set Interface command is received
{
   EP0BUF[0] = AlternateSetting;
   EP0BCH = 0;
   EP0BCL = 1;
   return(TRUE);            // Handled by user code
}

BOOL DR_GetStatus(void)
{
   return(TRUE);
}

BOOL DR_ClearFeature(void)
{
   return(TRUE);
}

BOOL DR_SetFeature(void)
{
   return(TRUE);
}

BOOL DR_VendorCmnd(void)
{
   return(TRUE);
}

//-----------------------------------------------------------------------------
// USB Interrupt Handlers
//   The following functions are called by the USB interrupt jump table.
//-----------------------------------------------------------------------------

// Setup Data Available Interrupt Handler
void ISR_Sudav(void) interrupt 0
{
   GotSUD = TRUE;            // Set flag
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUDAV;         // Clear SUDAV IRQ
}

// Setup Token Interrupt Handler
void ISR_Sutok(void) interrupt 0
{
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUTOK;         // Clear SUTOK IRQ
}

void ISR_Sof(void) interrupt 0
{
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSOF;            // Clear SOF IRQ
}

void ISR_Ures(void) interrupt 0
{
   // whenever we get a USB reset, we should revert to full speed mode
   pConfigDscr = pFullSpeedConfigDscr;
   ((CONFIGDSCR xdata *) pConfigDscr)->type = CONFIG_DSCR;
   pOtherConfigDscr = pHighSpeedConfigDscr;
   ((CONFIGDSCR xdata *) pOtherConfigDscr)->type = OTHERSPEED_DSCR;
   
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmURES;         // Clear URES IRQ
}

void ISR_Susp(void) interrupt 0
{
   Sleep = TRUE;
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUSP;
}

void ISR_Highspeed(void) interrupt 0
{
   if (EZUSB_HIGHSPEED())
   {
      pConfigDscr = pHighSpeedConfigDscr;
      ((CONFIGDSCR xdata *) pConfigDscr)->type = CONFIG_DSCR;
      pOtherConfigDscr = pFullSpeedConfigDscr;
      ((CONFIGDSCR xdata *) pOtherConfigDscr)->type = OTHERSPEED_DSCR;
   }

   EZUSB_IRQ_CLEAR();
   USBIRQ = bmHSGRANT;
}
void ISR_Ep0ack(void) interrupt 0
{
}
void ISR_Stub(void) interrupt 0
{
}
void ISR_Ep0in(void) interrupt 0
{
}
void ISR_Ep0out(void) interrupt 0
{
}
void ISR_Ep1in(void) interrupt 0
{


}
void ISR_Ep1out(void) interrupt 0
{
}
void ISR_Ep2inout(void) interrupt 0
{
}
void ISR_Ep4inout(void) interrupt 0
{
}
void ISR_Ep6inout(void) interrupt 0
{
}
void ISR_Ep8inout(void) interrupt 0
{
}
void ISR_Ibn(void) interrupt 0
{
}
void ISR_Ep0pingnak(void) interrupt 0
{
}
void ISR_Ep1pingnak(void) interrupt 0
{
}
void ISR_Ep2pingnak(void) interrupt 0
{
}
void ISR_Ep4pingnak(void) interrupt 0
{
}
void ISR_Ep6pingnak(void) interrupt 0
{
}
void ISR_Ep8pingnak(void) interrupt 0
{
}
void ISR_Errorlimit(void) interrupt 0
{
}
void ISR_Ep2piderror(void) interrupt 0
{
}
void ISR_Ep4piderror(void) interrupt 0
{
}
void ISR_Ep6piderror(void) interrupt 0
{
}
void ISR_Ep8piderror(void) interrupt 0
{
}
void ISR_Ep2pflag(void) interrupt 0
{
}
void ISR_Ep4pflag(void) interrupt 0
{
}
void ISR_Ep6pflag(void) interrupt 0
{
}
void ISR_Ep8pflag(void) interrupt 0
{
}
void ISR_Ep2eflag(void) interrupt 0
{
}
void ISR_Ep4eflag(void) interrupt 0
{
}
void ISR_Ep6eflag(void) interrupt 0
{
}
void ISR_Ep8eflag(void) interrupt 0
{
}
void ISR_Ep2fflag(void) interrupt 0
{
}
void ISR_Ep4fflag(void) interrupt 0
{
}
void ISR_Ep6fflag(void) interrupt 0
{
}
void ISR_Ep8fflag(void) interrupt 0
{
}
void ISR_GpifComplete(void) interrupt 0
{
}
void ISR_GpifWaveform(void) interrupt 0
{
}
