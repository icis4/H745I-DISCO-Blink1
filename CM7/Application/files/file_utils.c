/*
 * file_utils.c
 *
 *  Created on: Jan 29, 2023
 *      Author: iic
 */
#include "main.h"
#include <errno.h>
#include <string.h>
#include "fatfs.h"

#include "files/file_utils.h"

int diskfree(char *path, DWORD *total_kb, DWORD *free_kb) {
	FATFS *fs;
	DWORD fre_clust, fre_sect, tot_sect;
	FRESULT result;

	/* Get volume information and free clusters of drive 0:/ */
	if (strcmp(path, SDRAMDISKPath) == 0 || strncasecmp(path, "ram:", 4) == 0) {
		fs = &SDRAMDISKFatFS;
		result = f_getfree(SDRAMDISKPath, &fre_clust, &fs);

		/* Get total sectors and free sectors */
		tot_sect = (fs->n_fatent - 2) * fs->csize;
		fre_sect = fre_clust * fs->csize;

		/* Print the free space (assuming 512 bytes/sector) */
		*total_kb = tot_sect / 2;
		*free_kb = fre_sect / 2;

		/* Get volume information and free clusters of drive 1:/ */
	}
#if 0
	else if (strcmp(path, USERPath) == 0
			|| strncasecmp(path, "flash:", 6) == 0) {
		fs = &USERFatFS;
		result = f_getfree(USERPath, &fre_clust, &fs);

		/* Get total sectors and free sectors */
		tot_sect = (fs->n_fatent - 2) * fs->csize;
		fre_sect = fre_clust * fs->csize;

		/* Print the free space (assuming 4096 bytes/sector) */
		*total_kb = tot_sect * 4;
		*free_kb = fre_sect * 4;

		/* Get volume information and free clusters of drive 2:/ */
	} else if (strcmp(path, SDPath) == 0
			|| strncasecmp(path, "sd:", 3) == 0) {
		fs = &SDFatFS;
		result = f_getfree(SDPath, &fre_clust, &fs);

		/* Get total sectors and free sectors */
		tot_sect = (fs->n_fatent - 2) * fs->csize;
		fre_sect = fre_clust * fs->csize;

		/* Print the free space (assuming 512 bytes/sector) */
		*total_kb = tot_sect / 2;
		*free_kb = fre_sect / 2;

	} */
#endif
	else {
		*total_kb = -1;
		*free_kb = -1;
		errno = __ELASTERROR + FR_EXIST;
		return -1;
	}

	if (result != FR_OK) {
		errno = __ELASTERROR + result;
		return -1;
	}

	return 0;
}
