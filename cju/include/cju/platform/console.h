#pragma once

#include "common.h"

/**
 * All console color types for the cju console.
 */
typedef enum CuConsoleColor
{
	CU_CONSOLE_COLOR_DEFAULT = 0,
	CU_CONSOLE_COLOR_BLACK,
	CU_CONSOLE_COLOR_RED,
	CU_CONSOLE_COLOR_GREEN,
	CU_CONSOLE_COLOR_YELLOW,
	CU_CONSOLE_COLOR_BLUE,
	CU_CONSOLE_COLOR_MAGENTA,
	CU_CONSOLE_COLOR_CYAN,
	CU_CONSOLE_COLOR_WHITE,
	CU_CONSOLE_COLOR_RESET,
} CuConsoleColor;

typedef struct CuConsoleConfig
{
	CuConsoleColor textColor;
} CuConsoleConfig;

/**
 * Sets the console configuration.
 *
 * @param config The console configuration to set.
 */
void cuConsoleConfig(CuConsoleConfig config);

/**
 * Specific method to set the console color.
 *
 * @param color The color to set.
 */
void cuConsoleSetColor(CuConsoleColor color);

/**
 * Prints a message to the console.
 *
 * @param message The message to print.
 */
void cuConsolePrint(const char* message);

/**
 * Prints a message to the console followed by a newline.
 *
 * @param message The message to print.
 */
void cuConsolePrintLn(const char* message);

/**
 * Prints a formatted message to the console.
 *
 * @param format The format string.
 * @param ... The format arguments.
 */
void cuConsolePrintFormat(const char* format, ...);

/**
 * Prints a formatted message to the console followed by a newline.
 *
 * @param format The format string.
 * @param ... The format arguments.
 */
void cuConsolePrintLnFormat(const char* format, ...);

/**
 * Buffers a formatted string into the provided buffer.
 *
 * @param buffer The buffer to write the formatted string into.
 * @param bufferSize The size of the buffer.
 * @param format The format string.
 * @param ... The format arguments.
 */
void cuBufferedString(char* buffer, u32 bufferSize, const char* format, ...);