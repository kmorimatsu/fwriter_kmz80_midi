#include "main.h"

unsigned int NVMUnlock (unsigned int nvmop){
	unsigned int status;
	// Suspend or Disable all Interrupts
	asm volatile ("di %0" : "=r" (status));
	// Enable Flash Write/Erase Operations and Select
	// Flash operation to perform
	NVMCON = nvmop;
	// Write Keys
	NVMKEY = 0xAA996655;
	NVMKEY = 0x556699AA;
	// Start the operation using the Set Register
	NVMCONSET = 0x8000;
	// Wait for operation to complete
	while (NVMCON & 0x8000);
	// Restore Interrupts
	if (status & 0x00000001) asm volatile ("ei");
	else asm volatile ("di");
	// Disable NVM write enable
	NVMCONCLR = 0x0004000;
	// Return WRERR and LVDERR Error Status Bits
	return (NVMCON & 0x3000);
}

unsigned int NVMWriteWord (void* address, unsigned int data){
	unsigned int res;
	// Load data into NVMDATA register
	NVMDATA = data;
	// Load address to program into NVMADDR register
	NVMADDR = (unsigned int) address;
	// Unlock and Write Word
	res = NVMUnlock (0x4001);
	// Return Result
	return res;
}

unsigned int NVMErasePage(void* address){
	unsigned int res;
	// Set NVMADDR to the Start Address of page to erase
	NVMADDR = (unsigned int) address;
	// Unlock and Erase Page
	res = NVMUnlock(0x4004);
	// Return Result
	return res;
}

unsigned int NVMErasePFM(void){
	unsigned int res;
	// Unlock and Erase Program Flash
	res = NVMUnlock(0x4005);
	// Return Result
	return res;
}
