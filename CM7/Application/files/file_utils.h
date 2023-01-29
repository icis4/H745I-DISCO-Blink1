/*
 * file_utils.h
 *
 *  Created on: Jan 29, 2023
 *      Author: iic
 */

#ifndef FILE_UTILS_H_
#define FILE_UTILS_H_

#ifdef  __cplusplus
extern "C"
{
#endif

extern int diskfree(char *path, DWORD *total_kb, DWORD *free_kb);

#ifdef  __cplusplus
}
#endif


#endif /* FILES_FILE_UTILS_H_ */
