#define _SUPPRESS_PLIB_WARNING 1

#include "usb.h"
#include "usb_host_msd.h"
#include "usb_host_msd_scsi.h"
#include "FSIO.h"
#include "main.h"

// Configuration Bits
// Note: 4 MHz crystal is used. CPU clock is 48 MHz.
#pragma config FUSBIDIO  = OFF          // Not using USBID (pin #14, RB5)
#pragma config FVBUSONIO = OFF          // Not using VBUSON (pin #25, RB14)
#pragma config DEBUG = OFF

#pragma config UPLLEN   = ON            // USB PLL Enabled
#pragma config FPLLMUL  = MUL_24        // PLL Multiplier
#pragma config UPLLIDIV = DIV_1         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_1         // PLL Input Divider
#pragma config FPLLODIV = DIV_2         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
//#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx1      // ICE/ICD Comm Channel Select
#pragma config JTAGEN   = OFF           // JTAG disabled

extern volatile BOOL deviceAttached;
const char __attribute__((address(0xA000FFF0))) const hexfilename[16]="test.hex";

void blink_led(int num){
	volatile int i,j;
	TRISBbits.TRISB0=0;
	TRISBbits.TRISB1=0;
	LATBbits.LATB0=0;
	LATBbits.LATB1=1;
	while(1){
		for(j=0;j<num;j++){
			LATBbits.LATB0=0;
			LATBbits.LATB1=1;
			for(i=0;i<500000;i++);
			LATBbits.LATB0=1;
			LATBbits.LATB1=0;
			for(i=0;i<500000;i++);
		}
		for(i=0;i<500000;i++);
	}
}

int coretimer(void){
	// mfc0 v0,Count
	asm volatile("mfc0 $v0,$9");
}

int main(void)
{
	SearchRec sr;
	int i,filenum,cursor;
	volatile int ct;
    // Enable the cache for the best performance
    CheKseg0CacheOn();
	// Enable interrupt
	IFS0=0;
	IFS1=0;
	IEC0=0;
	IEC1=0;
    INTEnableSystemMultiVectoredInt();
	// Initialize USB
    init_usb();

	// Connect to USB memory
	ct=coretimer()+24000000*5;
	while(1){
		USBTasks();
		if (USBHostMSDSCSIMediaDetect()) {
			if (FSInit()) break;
		} else if (ct-coretimer()<0) {
			// Time out (5 sec)
			blink_led(1);
		}
	}
	deviceAttached = TRUE;

	// Load hex file to flash and run
	load_hexfile_and_run((char*)&hexfilename[0]);
}
