#pragma once

#include "common.h"

typedef enum CuFileMode
{
	CU_FILE_MODE_READ,
	CU_FILE_MODE_WRITE,	 ///< Automatically creates the file if it does not exist
	CU_FILE_MODE_APPEND, ///< Automatically creates the file if it does not exist
} CuFileMode;

typedef struct CuFile
{
	void* pPlatformData; ///< Platform-specific data for file handling

	const char* filePath;
	u8*			buffer;
	u32			size;
	CuFileMode	mode;	///< Must be defined when opening the file
	b8			isOpen; ///< Indicates if the file is currently open, can be check after cuFileOpen
} CuFile;

/**
 * Open a file and read its contents into memory.
 *
 * @param filePath The path to the file to open.
 * @param mode The mode in which to open the file (read, write, append).
 * @return A pointer to a CuFile structure containing the file data.
 */
CU_PLATFORM_API_DEFINE(CuFile*, cuFileOpen, const char* filePath, CuFileMode mode);

/**
 * Write data to a previously opened file.
 *
 * @param pFile Pointer to the CuFile structure representing the file.
 * @param pData Pointer to the data to write to the file.
 * @param dataSize The size of the data to write, in bytes.
 */
CU_PLATFORM_API_DEFINE(void, cuFileWrite, const CuFile* pFile, const u8* pData, u32 dataSize);

/**
 * Close a previously opened file and free its resources.
 *
 * @param pFile Pointer to the CuFile structure to close.
 */
CU_PLATFORM_API_DEFINE(void, cuFileClose, CuFile* pFile);