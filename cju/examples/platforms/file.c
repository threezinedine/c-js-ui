#include <cju/cju.h>

int main(void)
{
	const char* fileName = "example.txt";

	CuFile* pFile = CU_PLATFORM_API(cuFileOpen)(fileName, CU_FILE_MODE_WRITE);
	if (!pFile->isOpen)
	{
		return -1;
	}

	const char* message = "Hello, C-JS-UI File Handling!";
	CU_PLATFORM_API(cuFileWrite)(pFile, (const u8*)message, cuStrLength(message));
	CU_PLATFORM_API(cuFileClose)(pFile);

	pFile = CU_PLATFORM_API(cuFileOpen)(fileName, CU_FILE_MODE_READ);
	if (!pFile->isOpen)
	{
		return -1;
	}

	// Print the file contents to the console
	cuPrintLn((const char*)pFile->buffer);

	return 0;
}