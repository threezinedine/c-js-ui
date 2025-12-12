#pragma once

#include "common.h"

/**
 * @brief Enumeration of exception codes used in the CJU platform.
 */
typedef enum CuExceptionCode
{
	CU_EXCEPTION_CODE_UNKNOWN = 1,
	CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS,
	// Add more exception codes as needed.
} CuExceptionCode;

/**
 * Converts a CuExceptionCode to its corresponding string representation.
 *
 * @param code The exception code to convert.
 *
 * @return A string representation of the exception code.
 */
const char* cuConvertExceptionCodeToString(CuExceptionCode code);

/**
 * Raises an exception with the specified code and message.
 *
 * @param code    The exception code.
 * @param message The exception message.
 * @param file    The source file where the exception was raised.
 * @param line    The line number in the source file where the exception was raised.
 */
void cuRaiseException(CuExceptionCode code, const char* message, const char* file, u32 line);

#define CU_RAISE_EXCEPTION(code, message, ...)                                                                         \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[CU_BUFFER_SIZE];                                                                                   \
		cuBufferedString(buffer, sizeof(buffer), message, ##__VA_ARGS__);                                              \
		cuRaiseException((code), (buffer), __FILE__, __LINE__);                                                        \
	} while (CU_FALSE)
