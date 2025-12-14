#pragma once

#include "common.h"

/**
 * Compares two null-terminated strings for equality.
 *
 * @param pStrA Pointer to the first string.
 * @param pStrB Pointer to the second string.
 *
 * @return CU_TRUE if the strings are equal, CU_FALSE otherwise.
 */
b8 cuStrEqual(const char* pStrA, const char* pStrB);

/**
 * Calculates the length of a null-terminated string.
 *
 * @param pStr Pointer to the string.
 *
 * @return The length of the string, excluding the null terminator.
 */
u32 cuStrLength(const char* pStr);