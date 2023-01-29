/*
 * fatfs.c
 *
 *  Created on: Jan 29, 2023
 *      Author: iic
 */

#include "fatfs.h"
#include <stdlib.h>

extern uint8_t retSDRAMDISK;    /* Return value for SDRAMDISK */
extern char SDRAMDISKPath[4];   /* SDRAMDISK logical drive path */
extern FATFS SDRAMDISKFatFS;    /* File system object for SDRAMDISK logical drive */
extern FIL SDRAMDISKFile;       /* File object for SDRAMDISK */

uint8_t workBuffer[_MAX_SS];

void MX_FATFS_Init(void)
{
	/*## FatFS: Link the SDRAMDISK driver ###########################*/
	retSDRAMDISK = FATFS_LinkDriver(&SDRAMDISK_Driver, SDRAMDISKPath);

	/* FR_NO_FILESYSTEM Bug workaround*/
	FATFS *fs;
	fs = malloc(sizeof(FATFS));
	f_mount(fs, "", 0); /* Mount default drive */
	f_mount(NULL, "", 0); /* Unmount default drive */
	free(fs);
	/* FR_NO_FILESYSTEM Bug */

	if (retSDRAMDISK == FR_OK) {

		/* additional user code for init */
		retSDRAMDISK = f_mount(&SDRAMDISKFatFS, SDRAMDISKPath, 0);
		if (retSDRAMDISK == FR_OK)
			retSDRAMDISK = f_mkfs(SDRAMDISKPath, FM_ANY, 0, workBuffer,
					sizeof(workBuffer));
	}
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  return 0;
}


