#include "cju/platform/platform.h"
#include <stdio.h>

typedef struct CuPlatformFile
{
	FILE* handle;
} CuPlatformFile;

CuFile* cuFileOpen_default(const char* filePath, CuFileMode mode)
{
	CuFile* pFile = (CuFile*)CU_PLATFORM_API(cuAllocate)(sizeof(CuFile));
	CU_ASSERT(pFile != CU_NULL);

	CU_PLATFORM_API(cuMemorySet)(pFile, 0, sizeof(CuFile));
	pFile->filePath = filePath;
	pFile->mode		= mode;

	CuPlatformFile* pPlatformFile = (CuPlatformFile*)CU_PLATFORM_API(cuAllocate)(sizeof(CuPlatformFile));
	CU_ASSERT(pPlatformFile != CU_NULL);

	pFile->pPlatformData = pPlatformFile;

	switch (mode)
	{
	case CU_FILE_MODE_READ:
		pPlatformFile->handle = fopen(filePath, "rb");
		break;
	case CU_FILE_MODE_WRITE:
		pPlatformFile->handle = fopen(filePath, "wb");
		break;
	case CU_FILE_MODE_APPEND:
		pPlatformFile->handle = fopen(filePath, "ab");
		break;
	default:
		CU_UNREACHABLE();
		break;
	}

	if (pPlatformFile->handle == CU_NULL)
	{
		return pFile;
	}

	pFile->isOpen = CU_TRUE;

	if (mode == CU_FILE_MODE_READ)
	{
		fseek(pPlatformFile->handle, 0, SEEK_END);
		pFile->size = ftell(pPlatformFile->handle);
		fseek(pPlatformFile->handle, 0, SEEK_SET);

		pFile->buffer = (u8*)CU_PLATFORM_API(cuAllocate)(pFile->size);
		CU_ASSERT(pFile->buffer != CU_NULL);

		size_t bytesRead = fread(pFile->buffer, 1, pFile->size, pPlatformFile->handle);
		CU_ASSERT(bytesRead == pFile->size);
	}

	return pFile;
}

void cuFileWrite_default(const CuFile* pFile, const u8* pData, u32 dataSize)
{
	CU_ASSERT(pFile != CU_NULL);
	CU_ASSERT(pFile->isOpen == CU_TRUE);
	CU_ASSERT(pFile->pPlatformData != CU_NULL);

	CuPlatformFile* pPlatformFile = (CuPlatformFile*)pFile->pPlatformData;
	size_t			bytesWritten  = fwrite(pData, 1, dataSize, pPlatformFile->handle);
	CU_ASSERT(bytesWritten == dataSize);
}

void cuFileClose_default(CuFile* pFile)
{
	CU_ASSERT(pFile != CU_NULL);
	CU_ASSERT(pFile->pPlatformData != CU_NULL);
	CU_ASSERT(pFile->isOpen == CU_TRUE);

	CuPlatformFile* pPlatformFile = (CuPlatformFile*)pFile->pPlatformData;

	if (pPlatformFile->handle != CU_NULL)
	{
		fclose(pPlatformFile->handle);
		pPlatformFile->handle = CU_NULL;
	}

	CU_PLATFORM_API(cuFree)(pPlatformFile, sizeof(CuPlatformFile));

	if (pFile->buffer != CU_NULL)
	{
		CU_PLATFORM_API(cuFree)(pFile->buffer, pFile->size);
		pFile->buffer = CU_NULL;
		pFile->size	  = 0;
	}

	CU_PLATFORM_API(cuFree)(pFile, sizeof(CuFile));
	pFile = CU_NULL;
}