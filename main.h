/*
   This file is provided under the LGPL license ver 2.1.
   Written by Katsumi.
   https://github.com/kmorimatsu/
*/

#include <xc.h>
#include <sys/attribs.h>

extern int g_temp;

void blink_led(int num);

void load_hexfile_and_run(char* filename);

void init_usb(void);

unsigned int NVMUnlock (unsigned int nvmop);
unsigned int NVMWriteWord (void* address, unsigned int data);
unsigned int NVMErasePage(void* address);

#define FLASH_ADDRESS 0xbd000000
#define FLASH_BOOTLOADER_LENGTH 0
#define FLASH_APP_ADDRESS (FLASH_ADDRESS + FLASH_BOOTLOADER_LENGTH)
#define FLASH_APP_PAGENUM ((0x40000-FLASH_BOOTLOADER_LENGTH)>>10)
