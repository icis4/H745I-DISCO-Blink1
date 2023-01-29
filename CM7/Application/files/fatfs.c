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

uint8_t retSRAMDISK;    /* Return value for SRAMDISK */

void MX_FATFS_Init(void)
{
	uint8_t* workBuffer = malloc(_MAX_SS);

  /*## FatFS: Link the SDRAMDISK driver ###########################*/
  retSDRAMDISK = FATFS_LinkDriver(&SDRAMDISK_Driver, SDRAMDISKPath);

  /* FR_NO_FILESYSTEM Bug workaround*/
	  FATFS* fs;
	  fs = malloc(sizeof(FATFS));
	  f_mount(fs, "", 0); /* Mount default drive */
	  f_mount(NULL, "", 0); /* Unmount default drive */
	  free(fs);
	/* FR_NO_FILESYSTEM Bug */

	/* additional user code for init */
	retSRAMDISK = f_mount(&SDRAMDISKFatFS, SDRAMDISKPath, 0);
	if (retSRAMDISK == FR_OK)
		retSRAMDISK = f_mkfs(SDRAMDISKPath, FM_ANY, 0, workBuffer,
				sizeof(workBuffer));

	free(workBuffer);
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


